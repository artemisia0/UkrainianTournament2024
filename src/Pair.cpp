#include <Pair.hpp>
#include <utility>


Pair::Pair(Context* cntx,
		   std::unique_ptr<Projectile> projectile,
		   std::unique_ptr<Target> target)
	: AppElement(cntx)
	, mProjectile(std::move(projectile))
	, mTarget(std::move(target)) {
}

void Pair::update(double dt) {
	mProjectile->update(dt);
	mTarget->update(dt);

	if (distance(mProjectile->position, mTarget->position)
		<= mProjectile->radius + mTarget->radius) {
		mProjectile->isDestroyed = true;
		mTarget->isDestroyed = true;
	}
}

void Pair::draw(double dt) {
	mProjectile->draw(dt);
	mTarget->draw(dt);
}

void Pair::reset() {
	mTarget->reset();
	mProjectile->resetWithTarget(mTarget.get());
}

Target* Pair::target() {
	return mTarget.get();
}

Projectile* Pair::projectile() {
	return mProjectile.get();
}
