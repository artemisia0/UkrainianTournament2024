#pragma once

#include <vector>
#include <gl.hpp>
#include <AppElement.hpp>
#include <Context.hpp>
#include <Settings.hpp>
#include <Noise.hpp>


class Trajectory : public AppElement,
				   public std::vector<vec3> {
public:
	Trajectory(Context*);

	void reset();
	void resetIndex();

	virtual void update(double) override;
	virtual void draw(double) override;

	int index();

	vec3 position();

	void mixNoise(Noise&);


private:
	void advance();


public:
	vec3 color;


private:
	int mIndex;
	double mTimeSinceLastAdvance;
};
