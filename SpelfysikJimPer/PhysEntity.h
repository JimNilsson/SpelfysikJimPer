#ifndef _PHYSENTITY_H_
#define _PHYSENTITY_H_
#include "vec3f.h"

using namespace Vektor;

class PhysEntity
{
public:
	PhysEntity() {};
	~PhysEntity() {};

	float mass;
	float coeffDrag;
	float coeffMagnus;

	vec3f pos;
	vec3f linVel;
	vec3f angVel;
	vec3f gravForce;
	vec3f magnusForce;
	vec3f dragForce;
	vec3f resForce;
};


#endif
