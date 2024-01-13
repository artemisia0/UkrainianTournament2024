#include <Body.hpp>
#include <Util.hpp>
#include <cmath>
#include <Renderer.hpp>
#include <iostream>


Body::Body(Context* cntx)
	: AppElement(cntx)
	, position(0, 0, 0)
	, velocity(0, 0, 0)
	, color(1, 1, 1)
	, radius(10)
	, acceleration(1)
	, drawingMode(1)
	, isDestroyed(false)
	, toResetTrajectory(true) {
	trajectory = std::make_unique<Trajectory>(context());
}

void Body::update(double dt) {
	if (trajectory->empty()) {
		trajectory->push_back(position);
	}
	trajectory->update(dt);
	if (trajectory->size()) {
		position = trajectory->position();
	}
}

void Body::draw(double dt) {
	if (trajectory->size()) {
		trajectory->draw(dt);
	}

	if (drawingMode == 2) {  // Triangles.
		const float deltaYaw = M_PI/10;
		const float deltaPitch = M_PI/10;
		for (float yaw = 0.f; yaw <= M_PI*2.f; yaw += deltaYaw) {
			for (float pitch = 0.f; pitch <= M_PI*2.f; pitch += deltaPitch) {
				vec3 a = position+polarToCartesian(yaw,
												   pitch)*radius;
				vec3 b = position+polarToCartesian(yaw+deltaYaw,
												   pitch)*radius;
				vec3 c = position+polarToCartesian(yaw,
												   pitch+deltaPitch)*radius;
				vec3 d = position+polarToCartesian(yaw+deltaYaw,
												   pitch+deltaPitch)*radius;
				context()->renderer->appendToDrawList(a, b, c,
													  color, color, color);
				context()->renderer->appendToDrawList(a, b, d,
													  color, color, color);
			}
		}
	} else if (drawingMode == 0) {  // Points.
		const float deltaYaw = M_PI/20;
		const float deltaPitch = M_PI/20;
		for (float yaw = 0.f; yaw <= M_PI*2.f; yaw += deltaYaw) {
			for (float pitch = 0.f; pitch <= M_PI*2.f; pitch += deltaPitch) {
				vec3 a = position+polarToCartesian(yaw,
												   pitch)*radius;
				context()->renderer->appendToDrawList(a, color);
			}
		}
	} else if (drawingMode == 1) {  // Lines.
		const float deltaYaw = M_PI/20;
		const float deltaPitch = M_PI/20;
		for (float yaw = 0.f; yaw <= M_PI*2.f; yaw += deltaYaw) {
			for (float pitch = 0.f; pitch <= M_PI*2.f; pitch += deltaPitch) {
				vec3 a = position+polarToCartesian(yaw,
												   pitch)*radius;
				vec3 b = position+polarToCartesian(yaw+deltaYaw,
												   pitch)*radius;
				context()->renderer->appendToDrawList(a, b, color, color);
			}
		}
	} else if (drawingMode == 3) {  // None.
	} else {
		std::cerr << "Invalid drawing mode" << std::endl;
	}
}

void Body::step(double dt) {
	velocity += (vec3(0, context()->settings->gravity, 0)
				 + normalize(velocity)*acceleration)*float(dt);
	position += velocity*float(dt);
}

void Body::reset() {
	isDestroyed = false;

	if (not toResetTrajectory) {
	    trajectory->resetIndex();
	    return;
	}

	velocity = polarToCartesian(initialAngleInHP, initialAngleInVP)
		* float(initialSpeed);
	if (trajectory->size()) {
		position = trajectory->front();
	}
	trajectory->reset();
	trajectory->clear();
	for (int i = 0; i < context()->settings->trajectoryStepCount; ++i) {
		trajectory->push_back(position);
		step(context()->settings->trajectoryStep);
	}
}
