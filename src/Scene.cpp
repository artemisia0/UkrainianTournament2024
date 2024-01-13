#include <Scene.hpp>
#include <gl.hpp>
#include <Renderer.hpp>
#include <Settings.hpp>
#include <iostream>


Scene::Scene(Context* cntx)
	: AppElement(cntx)
	, mTimeSinceLastMousePicking(0.0) {
	mTerrain = std::make_unique<Terrain>(context());
	context()->terrain = mTerrain.get();
	context()->scene = this;
	mGUI = std::make_unique<GUI>(context());
}

Scene::~Scene() {
}

void Scene::handleEvent(const Event& event) {
	if (event.action == GLFW_PRESS) {
		if (event.key == GLFW_KEY_ESCAPE) {
			context()->settings->isCameraControlEnabled =
				not context()->settings->isCameraControlEnabled;
		} else if (event.key == GLFW_KEY_ENTER) {
			reset();
		} else if (event.key == GLFW_KEY_SPACE) {
			context()->settings->isPaused =
				not context()->settings->isPaused;
		}
	}

	if (context()->settings->isCameraControlEnabled) {
		context()->settings->cameraFieldOfView -= event.scrollY;
	}
}

void Scene::update(double dt) {
	mTimeSinceLastMousePicking += dt;
	
	int mouseButtonState = glfwGetMouseButton(context()->window,
											  GLFW_MOUSE_BUTTON_LEFT);
	if (mouseButtonState == GLFW_PRESS
		and context()->settings->isCameraControlEnabled) {
		int pairIndex = handleMousePickingPair();
		int obstacleIndex = handleMousePickingObstacle();
		const float mousePickingPeriod = 0.2f;
		if (pairIndex != -1) {
			if (mTimeSinceLastMousePicking > mousePickingPeriod) {
				mTimeSinceLastMousePicking = 0;
				mousePickingPairCallback(pairIndex);
			}
		}
		if (obstacleIndex != -1) {
			if (mTimeSinceLastMousePicking > mousePickingPeriod) {
				mTimeSinceLastMousePicking = 0;
				mousePickingObstacleCallback(obstacleIndex);
			}
		}
	}
	
	for (auto& p : mPairs) {
		p.update(dt);
	}
	for (auto& o : mObstacles) {
		o->update(dt);
	}
}

void Scene::draw(double dt) {
	mTerrain->draw(dt);
	for (auto& p : mPairs) {
		p.draw(dt);
	}
	for (auto& o : mObstacles) {
		o->draw(dt);
	}
	
	mGUI->draw(dt);

	checkForRemoval();
}

void Scene::reset() {
	for (auto& p : mPairs) {
		p.reset();
	}
	for (auto& o : mObstacles) {
		o->reset();
	}
}

std::vector<Pair>& Scene::pairs() {
	return mPairs;
}

std::vector<std::unique_ptr<Obstacle>>& Scene::obstacles() {
	return mObstacles;
}

int Scene::handleMousePickingPair() {
	auto touches = [&](Body* body, vec3 v)->bool {
		return distance(body->position, v) <= body->radius;
	};
	vec3 v = context()->settings->cameraPosition;
	vec3 dir = normalize(context()->settings->cameraDirection);
	for (int i = 0; i != 10000; ++i) {
		for (int pairIndex = 0; pairIndex < int(mPairs.size()); ++pairIndex) {
			if (touches(mPairs.at(pairIndex).target(), v)
				or touches(mPairs.at(pairIndex).projectile(), v)) {
				return pairIndex;
			}
		}
		v += dir;
	}
	return -1;
}

int Scene::handleMousePickingObstacle() {
	auto touches = [&](Body* body, vec3 v)->bool {
		return distance(body->position, v) <= body->radius;
	};
	vec3 v = context()->settings->cameraPosition;
	vec3 dir = normalize(context()->settings->cameraDirection);
	for (int i = 0; i != 10000; ++i) {
		for (int obstacleIndex = 0; obstacleIndex < int(mObstacles.size());
			 ++obstacleIndex) {
			if (touches(mObstacles.at(obstacleIndex).get(), v)
				or touches(mObstacles.at(obstacleIndex).get(), v)) {
				return obstacleIndex;
			}
		}
		v += dir;
	}
	return -1;
}

void Scene::mousePickingPairCallback(int pairIndex) {
	mGUI->mousePickingPairCallback(pairIndex);
}

void Scene::mousePickingObstacleCallback(int obstacleIndex) {
	mGUI->mousePickingObstacleCallback(obstacleIndex);
}

void Scene::checkForRemoval() {
	int pairToRemove = -1;
	int obstacleToRemove = -1;
	for (int i = 0; i < int(mPairs.size()); ++i) {
		if (mPairs.at(i).removed()) {
			pairToRemove = i;
			break;
		}
	}
	for (int i = 0; i < int(mObstacles.size()); ++i) {
		if (mObstacles.at(i)->removed()) {
			obstacleToRemove = i;
			break;
		}
	}
	if (pairToRemove >= 0) {
		mPairs.erase(mPairs.begin()+size_t(pairToRemove));
	}
	if (obstacleToRemove >= 0) {
		mObstacles.erase(mObstacles.begin()+size_t(obstacleToRemove));
	}
}
