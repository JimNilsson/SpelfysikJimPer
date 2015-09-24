#include "CannonBall.h"

extern SDL_Renderer* gRend;
extern SDL_Window* gWnd;
extern SDL_Surface* gSurf;

CannonBall::CannonBall(float mass, float radius, vec3f position, vec3f linearVel, vec3f angularVel)
{
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
	float linVelMagnitude = length(linVel - wind);
	float angVelMagnitude = length(angVel);
	
	float reynold = (AIR_DENSITY * linVelMagnitude * radius * 2) / AIR_VISCOSITY;
	coeffDrag = reynold < SPHERE_LAM_TURB_THRESHOLD ? CD_SPHERE_LAMINAR : CD_SPHERE_TURBULENT;
	coeffMagnus = linVelMagnitude > EPSILON ? (radius * length(angVel)) / (linVelMagnitude) : 0;

	dragForce = normalize(linVel - wind) * (-0.5f * AIR_DENSITY * area * coeffDrag * linVelMagnitude * linVelMagnitude);
	magnusForce = cross(normalize(linVel - wind), normalize(angVel)) * (0.5f * coeffMagnus * AIR_DENSITY * linVelMagnitude * linVelMagnitude * area);
	resForce = dragForce + magnusForce + gravForce;
	vec3f acc = resForce / mass;

	pos = pos + (linVel * dt) + ((acc * dt * dt) / 2.0f);
	linVel = linVel + (acc * dt);
	
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
	printf("Total force: (%.2f, %.2f, %.2f) N\n", resForce.x, resForce.y, resForce.z);

}