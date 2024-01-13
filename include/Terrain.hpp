#pragma once

#include <AppElement.hpp>


class Terrain : public AppElement {
public:
	Terrain(Context*);

	virtual void draw(double) override;
};
