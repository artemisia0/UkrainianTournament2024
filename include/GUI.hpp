#pragma once

#include <gl.hpp>
#include <AppElement.hpp>
#include <Translator.hpp>
#include <unordered_map>
#include <memory>
#include <string>
#include <imgui.hpp>


class Obstacle;
class Noise;
class Model;
class Body;
class Projectile;
class Target;
class Trajectory;

class GUI : public AppElement {
public:
	GUI(Context*);

	virtual void draw(double) override;

	void mousePickingPairCallback(int pairIndex);
	void mousePickingObstacleCallback(int obstacleIndex);


private:
	void drawOpenedWindows();
	void drawMainMenuBar();
	
	void drawPairEditor(int index);
	void drawObstacleEditor(int index);
	void drawOptionsMenu();
	void drawHelpPage();
	void drawBodyEditor(Body*, int index);

	void drawEditor(vec3&, const std::string&);
	void drawEditor(ivec2&, const std::string&);
	void drawEditor(bool&, const std::string&);
	void drawEditor(float&, const std::string&);
	void drawEditor(int&, const std::string&);
	void drawEditor(Model&, const std::string&);
	void drawEditor(Noise&);
	void drawEditor(Trajectory&);
	void drawEditor(Obstacle&);

	void show(Trajectory&);

	void drawTrajectoryVertexEditor(vec3&, int index);

	void createRandomPair();
	void createRandomObstacle();

	void setRandomProjectileParameters(Projectile*);
	void setRandomTargetParameters(Target*);
	void setRandomObstacleParameters(Obstacle*);

	std::string tr(const std::string&);

	void resetSettings();


private:
	std::unordered_map<int, bool> mIsPairEditorOpened;
	std::unordered_map<int, bool> mIsObstacleEditorOpened;
	std::unique_ptr<Translator> mTranslator;
	bool mIsOptionsMenuOpened;
	bool mIsHelpPageOpened;
	Language mLanguage;
	std::unordered_map<int, bool> mIsEditTargetTrajectoryOpened;
	std::unordered_map<int, bool> mIsShowProjectileTrajectoryOpened;
	std::unordered_map<int, bool> mIsEditTargetNoiseOpened;
	int mTheme;
	std::unordered_map<int, std::string> mHelpText;
};
