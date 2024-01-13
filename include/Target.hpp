#pragma once

#include <Body.hpp>
#include <Trajectory.hpp>
#include <memory>
#include <Noise.hpp>


class Target : public Body {
public:
	Target(Context*);

	virtual void reset() override;
	virtual void update(double) override;
	virtual void draw(double) override;


public:
	Noise noise;
};
