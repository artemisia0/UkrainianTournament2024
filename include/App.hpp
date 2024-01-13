#pragma once

#include <Settings.hpp>
#include <Scene.hpp>
#include <AppElement.hpp>
#include <gl.hpp>
#include <memory>
#include <Context.hpp>
#include <Renderer.hpp>


class App : public AppElement {
public:
	App();
	virtual ~App();

	void run();

	void resetSettings();


private:
	void handleEvents();

	virtual void handleEvent(const Event&) override;
	virtual void update(double) override;
	virtual void draw(double) override;

	std::unique_ptr<Context> defaultContext();
	std::unique_ptr<Scene> defaultScene();
	std::unique_ptr<Settings> defaultSettings();
	std::unique_ptr<Renderer> defaultRenderer();
	GLFWwindow* defaultWindow();


private:
	std::unique_ptr<Context> mContext;
	std::unique_ptr<Scene> mScene;
	std::unique_ptr<Settings> mSettings;
	std::unique_ptr<Renderer> mRenderer;
	GLFWwindow* mWindow;

	double mTimeSinceLastUpdate;
};
