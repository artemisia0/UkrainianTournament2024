#pragma once


class Context;
class Event;

class AppElement {
public:
	AppElement(Context*);
	virtual ~AppElement() = default;

	virtual void handleEvent(const Event&);
	virtual void update(double);
	virtual void draw(double);

	Context* context();
	void setContext(Context*);


private:
	Context* mContext;
};
