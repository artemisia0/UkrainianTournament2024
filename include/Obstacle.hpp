#pragma once

#include <Body.hpp>
#include <Removable.hpp>


class Obstacle : public Body, public Removable {
public:
	Obstacle(Context*);

	virtual void reset() override;
	virtual void update(double) override;
	virtual void draw(double) override;
};
