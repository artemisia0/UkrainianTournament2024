#pragma once

#include <Projectile.hpp>
#include <Target.hpp>
#include <memory>
#include <Removable.hpp>


class Pair : public AppElement, public Removable {
public:
	Pair(Context*,
		 std::unique_ptr<Projectile>,
		 std::unique_ptr<Target>);

	virtual void update(double) override;
	virtual void draw(double) override;
	virtual void reset();

	Target* target();
	Projectile* projectile();


private:
	std::unique_ptr<Projectile> mProjectile;
	std::unique_ptr<Target> mTarget;
};
