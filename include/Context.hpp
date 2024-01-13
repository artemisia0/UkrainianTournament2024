#pragma once


class Renderer;
class Settings;
class Scene;
class App;
class Terrain;
struct GLFWwindow;

class Context {
public:
	Renderer* renderer = nullptr;
	Settings* settings = nullptr;
	Scene* scene = nullptr;
	GLFWwindow* window = nullptr;
	App* app = nullptr;
	Terrain* terrain = nullptr;
};
