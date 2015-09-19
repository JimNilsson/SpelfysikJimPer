#ifndef _CANNONALL_H_
#define _CANNONALL_H_
#include "PhysEntity.h"
#include "vec3f.h"
#include <SDL.h>
#include <Windows.h>
#include "Constants.h"

using namespace Vektor;

class CannonBall : public PhysEntity
{
public:
	~CannonBall();
	CannonBall(float mass, float radius, vec3f position, vec3f linearVel, vec3f angularVel);
	
	void update(float dt, vec3f wind);
	void render(SDL_Surface* screen, SDL_Renderer* rend);

	float radius;
	float area;

	SDL_Surface* sprite;
	SDL_Rect rcSprite;
private:
	CannonBall() {};
};


#endif
