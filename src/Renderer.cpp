#include <Renderer.hpp>
#include <gl.hpp>
#include <Settings.hpp>
#include <Controls.hpp>


Renderer::Renderer(Context* cntx)
	: AppElement(cntx)
	, mPreviousMVP(mat4(1)) {
	mLineList = std::make_unique<DrawList>(context(), GL_LINES);
	mPointList = std::make_unique<DrawList>(context(), GL_POINTS);
	mTriangleList = std::make_unique<DrawList>(context(), GL_TRIANGLES);
}

Renderer::~Renderer() {
}

void Renderer::handleEvent(const Event&) {
}

void Renderer::update(double dt) {
	// Compute the MVP matrix from keyboard and mouse input
	computeMatricesFromInputs(
		context()->settings->isCameraControlEnabled,
		context(),
		dt);
	
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	
	const float smooth = 0.3;
	
	for (int i = 0; i != 4; ++i)  // Smoothing.
		for (int j = 0; j != 4; ++j)
			MVP[i][j] = (smooth*mPreviousMVP[i][j]
						 + MVP[i][j])/2.0;
	
	for (int i = 0; i != 4; ++i)  // Smoothing.
		for (int j = 0; j != 4; ++j)
			MVP[i][j] += mPreviousMVP[i][j]*smooth;
	mPreviousMVP = MVP;

	mLineList->setMVP(MVP);
	mPointList->setMVP(MVP);
	mTriangleList->setMVP(MVP);
}

void Renderer::draw(double dt) {
	mTriangleList->draw(dt);
	mLineList->draw(dt);
	mPointList->draw(dt);
}

void Renderer::appendToDrawList(vec3 a, vec3 b, vec3 colorA, vec3 colorB) {
	mLineList->append(a, colorA);
	mLineList->append(b, colorB);
}

void Renderer::appendToDrawList(vec3 point, vec3 color) {
	mPointList->append(point, color);
}

void Renderer::newFrame() {
	auto clearColor = context()->settings->backgroundColor;
	glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	mLineList->newFrame();
	mPointList->newFrame();
	mTriangleList->newFrame();
}

void Renderer::appendToDrawList(vec3 a, vec3 b, vec3 c,
								vec3 colorA, vec3 colorB, vec3 colorC) {
	mTriangleList->append(a, colorA);
	mTriangleList->append(b, colorB);
	mTriangleList->append(c, colorC);
}
