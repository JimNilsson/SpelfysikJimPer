#include "CannonBall.h"

extern SDL_Renderer* gRend;
extern SDL_Window* gWnd;
extern SDL_Surface* gSurf;

CannonBall::CannonBall(float mass, float radius, vec3f position, vec3f linearVel, vec3f angularVel)
{
	ZeroMemory(this, sizeof(CannonBall));
	launch = false;
	this->mass = mass;
	this->radius = radius;
	pos = position;
	linVel = linearVel;
	angVel = angularVel;
	area = PI * radius * radius;
	gravForce = vec3f(0, 0, -(mass*GRAVACC));
	sprite = NULL;
	spriteTex = NULL;

	sprite = SDL_LoadBMP("ball.bmp");
	spriteTex = SDL_CreateTextureFromSurface(gRend, sprite);
	SDL_FreeSurface(sprite);
}

CannonBall::~CannonBall()
{
	if (spriteTex != NULL)
		SDL_DestroyTexture(spriteTex);
}

void CannonBall::update(float dt, vec3f wind)
{
	resForce = fricForce = magnusForce = dragForce = vec3f(0,0,0);
	float linVelMagnitude = length(linVel - wind);
	float angVelMagnitude = length(angVel);
	
	if (pos.z - radius > 0.0f)//Only consider magnusforce if airborne
	{
		coeffMagnus = linVelMagnitude > EPSILON ? (radius * length(angVel)) / (linVelMagnitude) : 0;
		magnusForce = cross(normalize(angVel), normalize(linVel - wind)) * (0.5f * coeffMagnus * AIR_DENSITY * linVelMagnitude * linVelMagnitude * area);
	}
	if (pos.z - radius <= 0.0f && linVel.z < 0.01f) //We have collision with ground
	{
		float e = 0.60f; //Coefficient of restitution
		float f = 1.16f; //Coefficient of friction
		float fr = 0.08f; //Coefficient of rollfriction
		vec3f ep = vec3f(0.0f, 0.0f, 1.0f); //Line of action will be perpendicular to the ground
		//ep is also the normal of the ground plane
		vec3f vvn = projectOnPlane(linVel, ep);
		vec3f vwn = cross(ep, angVel) * radius;
		vec3f en = normalize(vvn + vwn) * -1.0f; //Direction of friction force
		
		if (linVel.z < -1.0f)//Bounce only for impact speed higher than -1m/s
		{
			pos.z = radius;
			float vp = dot(linVel, ep);
			float up = vp * e * -1.0f;
			linVel = linVel + (ep + (en * f)) * (up - vp);
			angVel = angVel + (cross(en, ep) * ((mass*radius*f*(up - vp)) / (2.0f*radius)));
			printf("Linvel = %.2f, %.2f, %.2f\n", linVel.x, linVel.y, linVel.z);
			printf("Angvel = %.2f, %.2f, %.2f\n", angVel.x, angVel.y, angVel.z);
		}
		else if (vvn != (vwn * -1.0f)) //glide phase
		{
			linVel.z = 0.0f;
			fricForce = en * f * GRAVACC * mass;//affects linear acceleration
			angAcc = cross(en, ep) * ((5.0f * f * GRAVACC) / (2.0f * radius));	
		}
		else //roll phase
		{
			linVel.z = 0.0f;
			fricForce = normalize(linVel) * fr * GRAVACC * mass * -1.0f;
			angAcc = cross(en, ep) * ((5.0f * fr * GRAVACC) / (2.0f * radius));
		}
	}

	//We always have drag (well unless there's no wind and the ball is standing still)
	float reynold = (AIR_DENSITY * linVelMagnitude * radius * 2) / AIR_VISCOSITY;
	coeffDrag = reynold < SPHERE_LAM_TURB_THRESHOLD ? CD_SPHERE_LAMINAR : CD_SPHERE_TURBULENT;
	dragForce = normalize(linVel - wind) * (-0.5f * AIR_DENSITY * area * coeffDrag * linVelMagnitude * linVelMagnitude);
	
	resForce = dragForce + magnusForce + gravForce + fricForce;
	linAcc = resForce / mass;

	pos = pos + (linVel * dt) + ((linAcc * dt * dt) / 2.0f);
	linVel = linVel + (linAcc * dt);
	angVel = angVel + angAcc * dt;
	
	
}


void CannonBall::render()
{
	
	SDL_Rect dest;
	dest.h = 2;
	dest.w = 2;
	dest.x = (int)pos.x;
	dest.y = -1 * (int)pos.z + (SCR_H / 2);
	SDL_RenderCopy(gRend, spriteTex, NULL, &dest);
	//SDL_BlitScaled(sprite, NULL, screen, &dest);
	dest.y = (int)pos.y + (SCR_H / 2) + (SCR_H / 4);
	SDL_RenderCopy(gRend, spriteTex, NULL, &dest);
	//SDL_BlitScaled(sprite, NULL, screen, &dest);
	
}

void CannonBall::printInfo()
{
	printf("Mass: %.2f kg\n", mass);
	printf("Radius: %.2f m\n", radius);
	printf("Velocity: (%.2f, %.2f, %.2f) m/s\n", linVel.x, linVel.y, linVel.z);
	printf("Angular Velocity: (%.2f, %.2f, %.2f) rad/s\n", angVel.x, angVel.y, angVel.z);
	printf("Drag force: (%.2f, %.2f, %.2f) N\n", dragForce.x, dragForce.y, dragForce.z);
	printf("Magnus force: (%.2f, %.2f, %.2f) N\n", magnusForce.x, magnusForce.y, magnusForce.z);
	printf("Friction force: (%.2f, %.2f, %.2f) N\n", fricForce.x, fricForce.y, fricForce.z);
	printf("Total force: (%.2f, %.2f, %.2f) N\n", resForce.x, resForce.y, resForce.z);
	if (linVel == (cross(angVel, vec3f(0, 0, 1)) * radius))
		printf("ROLLING\n");
	else
		printf("GLIDING\n");

}