#pragma once

#include <gl.hpp>


class Noise {
public:
	Noise();
	
	vec3 genVec3(vec3);
	float genFloat(vec3);


public:
	vec3 mix;
	vec3 frequency;
	int seed;
};
