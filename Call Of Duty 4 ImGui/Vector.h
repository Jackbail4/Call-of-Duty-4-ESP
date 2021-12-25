#pragma once
#include <math.h>

struct Vec2 {
	float x, y;
};
struct Vec3
{
	float x, y, z;

	Vec3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vec3() { x = y = z = 0; }
	float length() { return (float)sqrt(x * x + y * y + z * z); }
	float dotproduct(Vec3 dot) { return (x * dot.x + y * dot.y + z * dot.z); }
};
struct Vec3x3 {
	Vec3 vForward, vRight, vUpward;
};
