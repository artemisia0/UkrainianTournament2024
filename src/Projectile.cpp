#include <Projectile.hpp>
#include <Target.hpp>
#include <Util.hpp>
#include <cmath>
#include <Model.hpp>
#include <iostream>
#include <Scene.hpp>
#include <Obstacle.hpp>


Projectile::Projectile(Context* cntx)
	: Body(cntx)
	, toDrawModel(true)
	, mNoObstaclesOnTheWay(true) {
	mModel = defaultModel();
	position = vec3(0, 0, 0);
	mModel->modelPosition = position;
	drawingMode = 3;
}

void Projectile::update(double dt) {
	Body::update(dt);
	mModel->modelPosition = position;
	if (trajectory->index() < int(trajectory->size())-1) {
		mModel->modelDirection = trajectory->at(trajectory->index()+1)
			- trajectory->at(trajectory->index());
	}
}

void Projectile::draw(double dt) {
	if (not isDestroyed) {
		Body::draw(dt);
		if (toDrawModel) {
			mModel->draw(dt);
		}
	}
}

std::unique_ptr<Model> Projectile::defaultModel() {
	auto model = std::make_unique<Model>(context());
	if (not model->load("./assets/models/rocket.obj")) {
		std::cerr << "Failed to load 'assets/models/rocket.obj'" << std::endl;
	}
	return model;
}

// O(2*PI*2*n/d + 2*PI*2*n/d + n) =
// O(8*PI*n/d) = O(n/d)
// where PI is 3.1415, n is length of trajectory
// and d is an epsilon of angle computing.
void Projectile::resetWithTarget(Target* target) {
	initialAngleInVP = 0.f;
	const float eps = context()->settings->angleComputingEpsilon;
	if (eps < 0.0001) {
		return;
	}
	float smallestDistance = 1e6;
	float bestAngleInHP = 0.f;
	// Computing the best angle in horizontal plane.
	for (float angleInHP = 0.f; angleInHP <= 2*M_PI; angleInHP += eps) {
		initialAngleInHP = angleInHP;
		Body::reset();
		const float d = distanceFromTargetToTrajectory(
			target, trajectory.get());
		if (smallestDistance > d and mNoObstaclesOnTheWay) {
			smallestDistance = d;
			bestAngleInHP = angleInHP;
		}
	}
	initialAngleInHP = bestAngleInHP;
	smallestDistance = 1e6;
	float bestAngleInVP = 0.f;
	// Computing the best angle in vertical plane.
	for (float angleInVP = 0.f; angleInVP <= 2*M_PI; angleInVP += eps) {
		initialAngleInVP = angleInVP;
		Body::reset();
		const float d = distanceFromTargetToTrajectory(
			target, trajectory.get());
		if (smallestDistance > d and mNoObstaclesOnTheWay) {
			smallestDistance = d;
			bestAngleInVP = angleInVP;
		}
	}
	initialAngleInVP = bestAngleInVP;
	Body::reset();
}

// O(n) where n is length of trajectory.
float Projectile::distanceFromTargetToTrajectory(
	Target* target, Trajectory* trajectory) {
	mNoObstaclesOnTheWay = true;
	float d = 1e6;
	for (int i = 0; i < int(trajectory->size()); ++i) {
		d = std::min(d, float(distance(target->trajectory->at(i),
									   trajectory->at(i))));
		if (collidesObstacle(trajectory->at(i))) {
			mNoObstaclesOnTheWay = false;
		}
	}
	return d;
}

Model* Projectile::model() {
	return mModel.get();
}

bool Projectile::collidesObstacle(vec3 projectilePosition) {
	for (const auto& o : context()->scene->obstacles()) {
		if (distance(o->position, projectilePosition)
			< o->radius + radius) {
			return true;
		}
	}
	return false;
}
