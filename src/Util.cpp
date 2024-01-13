#include <Util.hpp>
#include <cmath>
#include <gl.hpp>
#include <algorithm>
#include <iostream>
#include <Body.hpp>
#include <Trajectory.hpp>


vec3 polarToCartesian(float yaw, float pitch)
{
    return normalize(vec3(
        std::cos(pitch)*std::sin(yaw),
        std::sin(pitch),
        std::cos(pitch)*std::cos(yaw)
    ));
}

bool isNearZero(float value)
{
    return abs(value) < 0.001;
}

GLenum glCheckError_(const char *file, int line)
{
    /*
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:
			error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:
			error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:
			error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:
			error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:
			error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:
			error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file
				  << " (" << line << ")" << std::endl;
	}
	return errorCode;
	*/
}

vec3 randomVec3(vec3 min, vec3 max) {
	return vec3(
		randomFloat(min.x, max.x),
		randomFloat(min.y, max.y),
		randomFloat(min.z, max.z));
}

float randomFloat(float min, float max) {
	float randomValue = ((float)rand()) / (float)RAND_MAX;
    float diff = max - min;
    float r = randomValue * diff;
    return min + r;
}

