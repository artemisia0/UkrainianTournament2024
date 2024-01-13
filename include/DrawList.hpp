#pragma once

#include <gl.hpp>
#include <Context.hpp>
#include <AppElement.hpp>
#include <vector>


class DrawList : public AppElement {
public:
	DrawList(Context*, GLenum primitiveType);
	virtual ~DrawList() = default;

	void append(vec3 vertex, vec3 color);
	void newFrame();
	void setMVP(mat4);
	virtual void draw(double) override;


private:
	GLenum mPrimitiveType;
	std::vector<vec3> mVertexBufferData;
	std::vector<vec3> mColorBufferData;
	GLuint mProgram;  // Shader program.
	GLuint mMatrixID;  // MVP matrix ID (inside shader).
	GLuint mVertexArray;
	GLuint mVertexBuffer;
	GLuint mColorBuffer;
	mat4 mMVP;
};
