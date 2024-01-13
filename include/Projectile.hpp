#pragma once

#include <Body.hpp>
#include <Model.hpp>


class Target;

class Projectile : public Body {
public:
	Projectile(Context*);

	void resetWithTarget(Target*);

	virtual void update(double) override;
	virtual void draw(double) override;

	Model* model();


private:
	float distanceFromTargetToTrajectory(Target*, Trajectory*);
	std::unique_ptr<Model> defaultModel();
	bool collidesObstacle(vec3 projectilePosition);


public:
	bool toDrawModel;
	

private:
	std::unique_ptr<Model> mModel;
	bool mNoObstaclesOnTheWay;
};
