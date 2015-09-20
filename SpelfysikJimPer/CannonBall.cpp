#include "CannonBall.h"

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
	rcSprite.h = 32;
	rcSprite.w = 32;
	rcSprite.x = 0;
	rcSprite.y = 0;

	SDL_Surface* temp;
	sprite = SDL_LoadBMP("ball.bmp");

}

CannonBall::~CannonBall()
{
	SDL_FreeSurface(sprite);
}

void CannonBall::update(float dt, vec3f wind)
{
	float linVelMagnitude = length(linVel - wind);
	float angVelMagnitude = length(angVel);
	
	float reynold = (AIR_DENSITY * linVelMagnitude * radius * 2) / AIR_VISCOSITY;
	coeffDrag = reynold < SPHERE_LAM_TURB_THRESHOLD ? CD_SPHERE_LAMINAR : CD_SPHERE_TURBULENT;
	coeffMagnus = (radius * length(angVel)) / linVelMagnitude;

	dragForce = normalize(linVel - wind) * (-0.5f * AIR_DENSITY * area * coeffDrag * linVelMagnitude * linVelMagnitude);
	magnusForce = cross(normalize(linVel - wind), normalize(angVel)) * (0.5 * coeffMagnus * AIR_DENSITY * linVelMagnitude * linVelMagnitude * area);
	resForce = dragForce + magnusForce + gravForce;
	vec3f acc = resForce / mass;

	pos = pos + (linVel * dt) + ((acc * dt * dt) / 2);
	linVel = linVel + (acc * dt);
	
}

void CannonBall::render(SDL_Surface* screen, SDL_Renderer* rend)
{
	SDL_Rect dest;
	dest.h = 2;
	dest.w = 2;
	dest.x = (int)pos.x;
	dest.y = -1 * (int)pos.z + (SCR_H / 2);
	SDL_BlitScaled(sprite, &rcSprite, screen, &dest);
	dest.y = (int)pos.y + (SCR_H / 2) + (SCR_H / 4);
	SDL_BlitScaled(sprite, &rcSprite, screen, &dest);
	
}

void CannonBall::printInfo()
{
	printf("Velocity: %.2f, %.2f, %.2f\n", linVel.x, linVel.y, linVel.z);
	printf("Total force: %.2f, %.2f, %.2f\n", resForce.x, resForce.y, resForce.z);

}