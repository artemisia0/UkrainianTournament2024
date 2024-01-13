#include <Model.hpp>
#include <iostream>
#include <fstream>
#include <Renderer.hpp>
#include <QuaternionUtil.hpp>


Model::Model(Context* cntx)
	: AppElement(cntx)
	, modelPosition(vec3(0))
	, modelDirection(0)
	, modelScaling(1) {
}

bool Model::load(const std::string& filename) {
	std::ifstream in(filename);
	if (not in) {
		std::cerr << "Failed to open file " + filename << std::endl;
		return false;
	}

	std::vector<vec3> vertices, colors;

	while (in) {
		std::string heading;
		in >> heading;
		if (heading == "v") {
			float x, y, z;
			float a, b, c;
			in >> x >> y >> z >> a >> b >> c;
			vertices.push_back(vec3(x, y, z));
			colors.push_back(vec3(a, b, c));
		} else if (heading == "f") {
			int a, b, c;
			in >> a >> b >> c;
			mVertices.push_back(vertices.at(a-1));
			mColors.push_back(colors.at(a-1));
			mVertices.push_back(vertices.at(b-1));
			mColors.push_back(colors.at(b-1));
			mVertices.push_back(vertices.at(c-1));
			mColors.push_back(colors.at(c-1));
		} else {  // Reading the rest of the line.
			std::string s;
			std::getline(in, s);
		}
	}

	return true;
}

void Model::draw(double) {
	mat4 translation = translate(mat4(1), modelPosition);
	mat4 rotation = toMat4(LookAt(modelDirection, vec3(0, 1, 0)))
		* toMat4(quat(vec3(radians(-270.f), 0, 0)));
	mat4 scaling = scale(mat4(1), modelScaling);
	mat4 modelMatrix = translation * rotation * scaling;
	
	assert(mVertices.size() % 3 == 0);
	assert(mColors.size() % 3 == 0);
	for (int i = 0; i+2 < int(mVertices.size());) {
		context()->renderer->appendToDrawList(
			vec3(modelMatrix*vec4(mVertices.at(i), 1)),
			vec3(modelMatrix*vec4(mVertices.at(i+1), 1)),
			vec3(modelMatrix*vec4(mVertices.at(i+2), 1)),
			mColors.at(i), mColors.at(i+1), mColors.at(i+2));
		i += 3;
	}
}
