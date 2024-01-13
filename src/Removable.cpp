#include <Removable.hpp>


Removable::Removable()
	: mRemoved(false) {
}

void Removable::remove() {
	mRemoved = true;
}

bool Removable::removed() {
	return mRemoved;
}
