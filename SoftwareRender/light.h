#ifndef __LIGHT__H__
#define __LIGHT__H__

#include "Common/geometry.h"

struct Light {
	vec3 color;
	vec3 direction;
	vec3 ambient;
	double intensity;

	Light() = default;

	Light(vec3 color, vec3 direction, vec3 ambient, int intensity = 1) : color(color), direction(direction.normalized()), ambient(ambient), intensity(intensity) {}
};

#endif