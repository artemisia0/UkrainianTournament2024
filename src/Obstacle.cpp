#include <Obstacle.hpp>


Obstacle::Obstacle(Context* cntx)
	: Body(cntx) {
}

void Obstacle::reset() {
}

void Obstacle::update(double) {
}

void Obstacle::draw(double dt) {
	Body::draw(dt);
}
