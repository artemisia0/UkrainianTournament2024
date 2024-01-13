#pragma once

#include <AppElement.hpp>
#include <Context.hpp>
#include <GUI.hpp>
#include <Pair.hpp>
#include <Terrain.hpp>
#include <Obstacle.hpp>


class Scene : public AppElement {
public:
	Scene(Context*);
	virtual ~Scene();

	virtual void handleEvent(const Event&) override;
	virtual void update(double) override;
	virtual void draw(double) override;

	std::vector<Pair>& pairs();
	std::vector<std::unique_ptr<Obstacle>>& obstacles();


private:
	void reset();  // When enter key is pressed.
	int handleMousePickingPair();
	int handleMousePickingObstacle();
	void mousePickingPairCallback(int pairIndex);
	void mousePickingObstacleCallback(int obstacleIndex);
	void checkForRemoval();


private:
	std::unique_ptr<GUI> mGUI;
	std::vector<Pair> mPairs;
	std::vector<std::unique_ptr<Obstacle>> mObstacles;
	float mTimeSinceLastMousePicking;
	std::unique_ptr<Terrain> mTerrain;
};
