#ifndef _ROPEBALL_H_
#define _ROPEBALL_H_
#include "PhysEntity.h"
#include "vec3f.h"
#include <SDL.h>
#include <Windows.h>
#include "Constants.h"
#include <iostream>

class RopeBall : public PhysEntity
{
public:
	RopeBall(float mass, float radius, float ropelength, vec3f anchorpoint);
	~RopeBall();
	float radius;
	float ropelen;
	vec3f anchorpoint;
	vec3f forceRope;
	vec3f forceTang;

	void update(float dt, vec3f wind);
	void render(SDL_Surface* screen, SDL_Renderer* rend);
private:
	RopeBall();
	SDL_Rect rcSprite;
	SDL_Surface* sprite;

};


#endif
