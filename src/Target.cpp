#include <Target.hpp>


Target::Target(Context* cntx)
	: Body(cntx) {
	drawingMode = 2;  // Triangles
}

void Target::update(double dt) {
	Body::update(dt);
	// following by trajectory if not context()->settings->isPaused
}

void Target::draw(double dt) {
	if (not isDestroyed) {
		Body::draw(dt);
	}
}

void Target::reset() {
	Body::reset();
	// Go to the start of the trajectory.
	trajectory->mixNoise(noise);
}
