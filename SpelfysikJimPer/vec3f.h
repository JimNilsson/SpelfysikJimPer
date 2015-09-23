#ifndef _VEC3F_H_
#define _VEC3F_H_
#include <math.h>
#define zeromem(x,y) memset(x,0,y);
namespace Vektor
{
	class vec3f
	{
	public:
		float x, y, z;
		vec3f() { x = 0; y = 0; z = 0; }
		vec3f(float a, float b, float c) { x = a; y = b; z = c; }
		vec3f(const vec3f& o) { x = o.x; y = o.y; z = o.z; }
		vec3f& operator=(const vec3f& o) 
		{ 
			if(&o != this)
				x = o.x; y = o.y; z = o.z; 
			return *this;
		}

		vec3f operator/(float d)
		{
			return vec3f(x / d, y / d, z / d);
		}
		vec3f operator*(float d)
		{
			return vec3f(x*d, y*d, z*d);
		}
		vec3f operator+(vec3f v)
		{
			return vec3f(x + v.x, y + v.y, z + v.z);
		}
		vec3f operator-(vec3f v)
		{
			return vec3f(x - v.x, y - v.y, z - v.z);
		}
	};
	vec3f normalize(vec3f v);
	float dot(vec3f v, vec3f u);
	float length(vec3f v);
	vec3f cross(vec3f v, vec3f u);
	vec3f projectOnVec(vec3f v, vec3f u);
	vec3f projectOnPlane(vec3f v, vec3f n);
}
#endif
