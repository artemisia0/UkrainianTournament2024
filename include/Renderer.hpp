#pragma once

#include <gl.hpp>
#include <imgui.hpp>
#include <DrawList.hpp>
#include <Event.hpp>
#include <AppElement.hpp>

#include <memory>


class Renderer : public AppElement {
public:
	Renderer(Context*);
	virtual ~Renderer();

	void newFrame();  // Clear draw lists.

	void appendToDrawList(vec3 a, vec3 b, vec3 c,
						  vec3 colorA, vec3 colorB, vec3 colorC);
	void appendToDrawList(vec3 a, vec3 b, vec3 colorA, vec3 colorB);
	void appendToDrawList(vec3 point, vec3 color);

	virtual void handleEvent(const Event&) override;
	virtual void update(double) override;
	virtual void draw(double) override;


private:
	std::unique_ptr<DrawList> mTriangleList;
	std::unique_ptr<DrawList> mLineList;
	std::unique_ptr<DrawList> mPointList;

	mat4 mPreviousMVP;
};
