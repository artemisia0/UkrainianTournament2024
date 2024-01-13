#pragma once


class Removable {
public:
	Removable();

	bool removed();
	void remove();


private:
	bool mRemoved;
};
