#include <Noise.hpp>
#include <stb_perlin.h>


Noise::Noise()
	: mix(1)
	, frequency(0.1)
	, seed(0) {
}

float Noise::genFloat(vec3 v) {
	return stb_perlin_noise3_seed(v.x, v.y, v.z, 0, 0, 0, seed);
}

vec3 Noise::genVec3(vec3 v) {
	v *= frequency;
	return mix * vec3(
		genFloat(vec3(-999, 123, v.x+50000)),
		genFloat(vec3(1000-v.y, -222, 4554)),
		genFloat(vec3(5000, v.z, 12312)));
}
