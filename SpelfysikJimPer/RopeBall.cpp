#include "RopeBall.h"
#include <math.h>
#include <DirectXMath.h>

extern SDL_Renderer* gRend;
extern SDL_Surface* gSurf;
extern SDL_Window* gWnd;

RopeBall::RopeBall(float mass, float radius, float ropelength, vec3f anchorpoint)
{
	this->mass = mass;
	this->radius = radius;
	this->ropelen = ropelength;
	this->anchorpoint = anchorpoint;
	area = PI * radius * radius;
	pos = anchorpoint + (vec3f(0, 0, -1) * ropelen);
	gravForce = vec3f(0, 0, -9.82f) * mass;
	forceRope = gravForce * (-1);
	forceTang = vec3f(0, 0, 0);
	linVel = vec3f(0, 0, 0);
	angVel = vec3f(0, 0, 0);
	sprite = NULL;
	spriteTex = NULL;


	sprite = SDL_LoadBMP("ball2.bmp");
	SDL_SetColorKey(sprite, SDL_TRUE, SDL_MapRGB(sprite->format, 0, 0xFF, 0xFF));
	spriteTex = SDL_CreateTextureFromSurface(gRend, sprite);
	SDL_FreeSurface(sprite);
}

RopeBall::~RopeBall()
{
	if (spriteTex != NULL)
		SDL_DestroyTexture(spriteTex);
}

void RopeBall::update(float dt, vec3f wind)
{
	vec3f atop = pos - anchorpoint;
	vec3f atopDir = normalize(atop);  //Also normal of tangential plane
	linVel = cross(angVel,atopDir) * ropelen;
	vec3f windDir = normalize(wind - linVel);
	float windMagnitude = length(wind - linVel);
	float reynold = (AIR_DENSITY * windMagnitude * radius * 2.0f) / AIR_VISCOSITY;
	coeffDrag = reynold < SPHERE_LAM_TURB_THRESHOLD ? CD_SPHERE_LAMINAR : CD_SPHERE_TURBULENT;
	
	dragForce = windDir * (0.5f * AIR_DENSITY * area * coeffDrag * windMagnitude * windMagnitude);
	
	//float angle = acos(dot(vec3f(0, 0, -1.0f), atopDir));
	//forceRope = atopDir * (mass * GRAVACC + (length(dragForce) * sinf(angle))) * (-1); //dont even need this
	vec3f dragForceTangent = projectOnPlane(dragForce, atopDir);
	vec3f gravForceTangent = projectOnPlane(gravForce, atopDir);
	vec3f forceTang = dragForceTangent + gravForceTangent;
	vec3f accTang = forceTang / mass;
	vec3f accAng = accTang / ropelen;
	accAng = cross(atopDir, accAng);
	vec3f rotvec = (angVel * dt);
	
	float ang = length(rotvec);
	if (ang > 0.0f)
	{
		DirectX::XMVECTOR rv = DirectX::XMVectorSet(rotvec.x, rotvec.y, rotvec.z, 0);
		DirectX::XMVECTOR ap = DirectX::XMVectorSet(atop.x, atop.y, atop.z, 0);
		DirectX::XMMATRIX rotmat = DirectX::XMMatrixRotationAxis(rv, ang);
		ap = DirectX::XMVector3TransformCoord(ap, rotmat);
		DirectX::XMFLOAT3 a;
		DirectX::XMStoreFloat3(&a, ap);
		pos = anchorpoint + vec3f(a.x, a.y, a.z);
	}
	angVel = angVel + (accAng * dt);
}


void RopeBall::render()
{
	SDL_Rect dest;
	dest.h = (int)(2.0f * radius);
	dest.w = (int)(2.0f * radius);
	dest.x = (int)(pos.x - radius);
	dest.y = -1 * (int)pos.z + (SCR_H / 2);
	SDL_RenderCopy(gRend, spriteTex, NULL, &dest);

	dest.y = (int)pos.y + (SCR_H / 2) + (SCR_H / 4) - radius;
	SDL_RenderCopy(gRend, spriteTex, NULL, &dest);

	SDL_Point a;
	SDL_Point b;
	a.x = (int)anchorpoint.x;
	a.y = (int)((SCR_H / 2) - anchorpoint.z);
	b.x = (int)pos.x;
	b.y = (int)((SCR_H / 2) - pos.z);
	SDL_SetRenderDrawColor(gRend, 0x00, 0xFF, 0x00, 0xFF);
	SDL_RenderDrawLine(gRend, a.x, a.y, b.x, b.y);
	a.y = (int)((SCR_H / 2) - anchorpoint.y) + (SCR_H / 4);
	b.y = (int)((SCR_H / 2) + pos.y) + (SCR_H / 4);
	SDL_RenderDrawLine(gRend, a.x, a.y, b.x, b.y);
}
