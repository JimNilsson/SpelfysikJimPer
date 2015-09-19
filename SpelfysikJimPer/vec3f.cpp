#include "Includes.h"

using namespace Vektor;

vec3f Vektor::normalize(vec3f v)
{
	float d = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
	return vec3f(v.x / d, v.y / d, v.z / d);
}

float Vektor::dot(vec3f v, vec3f u)
{
	return v.x*u.x + v.y*u.y + v.z*u.z;
}

vec3f Vektor::cross(vec3f v, vec3f u)
{
	return vec3f(v.y*u.z - u.y*v.z, -v.x*u.z + u.x*v.z, v.x*u.y - v.y*u.x);
}

vec3f Vektor::projectOnVec(vec3f v, vec3f u)
{
	return (u * (dot(v, u) / dot(u, u)));
}

vec3f Vektor::projectOnPlane(vec3f v, vec3f n)
{
	return v - projectOnVec(v, n);
}

float Vektor::length(vec3f v)
{
	return sqrt(dot(v, v));
}

