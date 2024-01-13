#pragma once

#include <Context.hpp>


void computeMatricesFromInputs(bool, Context*, double);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
