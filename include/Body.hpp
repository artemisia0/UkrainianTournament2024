#pragma once

#include <AppElement.hpp>
#include <gl.hpp>
#include <Trajectory.hpp>
#include <memory>


class Body : public AppElement {
public:
	Body(Context*);

	virtual void reset();

	virtual void update(double) override;
	virtual void draw(double) override;


private:
	void step(double);


public:
	vec3 position;
	vec3 velocity;
	float initialAngleInHP;
	float initialAngleInVP;
	float initialSpeed;
	vec3 color;
	float radius;
	float acceleration;
	int drawingMode;
	bool isDestroyed;
	bool toResetTrajectory;
	std::unique_ptr<Trajectory> trajectory;
};
