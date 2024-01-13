#include <Terrain.hpp>
#include <gl.hpp>
#include <Context.hpp>
#include <Renderer.hpp>
#include <Settings.hpp>


Terrain::Terrain(Context* cntx)
	: AppElement(cntx) {
}

void Terrain::draw(double) {
	const float s = 3000;
	vec3 v1(0, context()->settings->terrainHeight, -s);
	while (v1.z <= s) {
		vec3 color = context()->settings->terrainColor;
		vec3 d(s, 0, 0);
		context()->renderer->appendToDrawList(v1-d, v1+d, color, color);
		v1.z += 10;
	}

	vec3 v2(-s, context()->settings->terrainHeight, 0);
	while (v2.x <= s) {
		vec3 color = context()->settings->terrainColor;
		vec3 d(0, 0, s);
		context()->renderer->appendToDrawList(v2-d, v2+d, color, color);
		v2.x += 10;
	}
}
