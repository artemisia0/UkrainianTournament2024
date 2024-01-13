#include <AppElement.hpp>


AppElement::AppElement(Context* cntx)
	: mContext(cntx) {
}

void AppElement::handleEvent(const Event&) {
}

void AppElement::update(double) {
}

void AppElement::draw(double) {
}

Context* AppElement::context() {
	return mContext;
}

void AppElement::setContext(Context* cntx) {
	mContext = cntx;
}
