#include <Trajectory.hpp>
#include <Renderer.hpp>
#include <cmath>


Trajectory::Trajectory(Context* cntx)
	: AppElement(cntx)
	, color(1, 1, 1)
	, mIndex(0)
	, mTimeSinceLastAdvance(0.0) {
}

void Trajectory::resetIndex() {
    mIndex = 0;
}

void Trajectory::update(double dt) {
	const double advancePeriod = context()->settings->trajectoryAdvancePeriod;
	if (not context()->settings->isPaused) {
		mTimeSinceLastAdvance += dt;
	}
	while (mTimeSinceLastAdvance >= advancePeriod
		and advancePeriod > 0.001) {
		mTimeSinceLastAdvance -= advancePeriod;
		advance();
	}
}

void Trajectory::draw(double) {
	for (int i = 0; i < int(size())-1; ++i) {
		context()->renderer->appendToDrawList(
			at(i), at(i+1), color, color);
	}
}

void Trajectory::advance() {
	++mIndex;
	mIndex = std::min(mIndex, int(size())-1);
}

vec3 Trajectory::position() {
	return at(mIndex);
}

void Trajectory::reset() {
	clear();
	mIndex = 0;
}

int Trajectory::index() {
	return mIndex;
}

void Trajectory::mixNoise(Noise& noise) {
	// Starting from 1 cause it is not cool to mix noise to position.
	for (int i = 1; i < int(size()); ++i) {
		at(i) += noise.genVec3(at(i));
	}
}
