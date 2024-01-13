#include <DrawList.hpp>
#include <Shader.hpp>
#include <Util.hpp>


DrawList::DrawList(Context* cntx, GLenum primitiveType)
	: AppElement(cntx)
	, mPrimitiveType(primitiveType)
	, mMatrixID(0) {
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// Create and compile GLSL program from the shaders.
    mProgram =
		LoadShaders( "shaders/TransformVertexShader.vertexshader",
					 "shaders/TextureFragmentShader.fragmentshader" );
	
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);

	glGenBuffers(1, &mColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);

	glCheckError();

	mMatrixID = glGetUniformLocation(mProgram, "MVP");
}

void DrawList::append(vec3 vertex, vec3 color) {
	mVertexBufferData.push_back(vertex);
	mColorBufferData.push_back(color);
}

void DrawList::newFrame() {
	mVertexBufferData.clear();
	mColorBufferData.clear();
}

void DrawList::draw(double) {
	glUseProgram(mProgram);

    glBindVertexArray(mVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        mVertexBufferData.size()*sizeof(mVertexBufferData[0]),
        &mVertexBufferData[0],
        GL_STREAM_DRAW);
	
    glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        mColorBufferData.size()*sizeof(mColorBufferData[0]),
        &mColorBufferData[0],
        GL_STREAM_DRAW);

	glCheckError();
	
	glUniformMatrix4fv(mMatrixID, 1, GL_FALSE, &mMVP[0][0]);

	glCheckError();

	glEnableVertexAttribArray(0);

	glCheckError();

	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);

	glCheckError();

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	glCheckError();

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);
	glCheckError();
	glDrawArrays(mPrimitiveType, 0, mVertexBufferData.size());
	glCheckError();
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void DrawList::setMVP(mat4 m) {
	mMVP = m;
}
