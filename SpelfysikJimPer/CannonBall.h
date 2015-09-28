#ifndef _CANNONALL_H_
#define _CANNONALL_H_
#include "PhysEntity.h"
#include "vec3f.h"
#include <SDL.h>
#include <Windows.h>
#include "Constants.h"
#include <iostream>

using namespace Vektor;

class CannonBall : public PhysEntity
{
public:
	~CannonBall();
	CannonBall(float mass, float radius, vec3f position, vec3f linearVel, vec3f angularVel);
	
	void update(float dt, vec3f wind);
	void render();
	void printInfo();

	float radius;
	bool launch;

	SDL_Surface* sprite;
	SDL_Texture* spriteTex;

private:
	CannonBall() {};
	
};


#endif
