#pragma once

#include <gl.hpp>


class Body;
class Trajectory;

vec3 polarToCartesian(float yaw, float pitch);

float isAlmostZero(float);  // True if abs(value) is < 0.0001 for example.

GLenum glCheckError_(const char*, int);
#define glCheckError() glCheckError_(__FILE__, __LINE__)

int primitiveTypeCountVertices(GLenum);

vec3 randomVec3(vec3 min, vec3 max);
float randomFloat(float min, float max);
