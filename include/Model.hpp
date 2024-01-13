#pragma once

#include <AppElement.hpp>
#include <gl.hpp>
#include <vector>
#include <string>


class Model : public AppElement {
public:
	Model(Context*);

	// It is possible to load .obj files only.
	bool load(const std::string& filename);

	virtual void draw(double) override;


public:
	vec3 modelPosition;
	vec3 modelDirection;
	vec3 modelScaling;


private:
	std::vector<vec3> mVertices;
	std::vector<vec3> mColors;
};
