#include <Settings.hpp>
#include <fstream>
#include <iostream>

#define SETTINGS_FILENAME "___settings__.txt"


static std::istream& operator>>(std::istream& is, ivec2& v) {
	is >> v.x >> v.y;
	return is;
}

static std::istream& operator>>(std::istream& is, vec3& v) {
	is >> v.x >> v.y >> v.z;
	return is;
}

static std::ostream& operator<<(std::ostream& os, ivec2& v) {
	std::string e = "\n";
	os << v.x << e
	   << v.y << e;
	return os;
}

static std::ostream& operator<<(std::ostream& os, vec3& v) {
	std::string e = "\n";
		os << v.x << e
		   << v.y << e
		   << v.z << e;
	return os;
}

bool Settings::canBeLoaded() {
	std::ifstream in(SETTINGS_FILENAME);
	bool value = bool(in);
	in.close();
	
	return value;
}

void Settings::load() {
	std::ifstream in(SETTINGS_FILENAME);
	if (not in) {
		std::cerr << "Failed to open settings file" << std::endl;
		return;
	}

	in >> windowSize
	   >> isCameraControlEnabled
	   >> cameraAngleInHP
	   >> cameraAngleInVP
	   >> mouseSpeed
	   >> cameraDirection
	   >> cameraPosition
	   >> cameraSpeed
	   >> cameraFieldOfView
	   >> nearPlane
	   >> farPlane
	   >> trajectoryAdvancePeriod
	   >> isPaused
	   >> trajectoryStepCount
	   >> trajectoryStep
	   >> gravity
	   >> angleComputingEpsilon
	   >> minRandomTargetPosition
	   >> maxRandomTargetPosition
	   >> minRandomProjectilePosition
	   >> maxRandomProjectilePosition
	   >> minRandomTargetRadius
	   >> maxRandomTargetRadius
	   >> minRandomProjectileRadius
	   >> maxRandomProjectileRadius
	   >> minRandomTargetInitialAngleInHP
	   >> maxRandomTargetInitialAngleInHP
	   >> minRandomTargetInitialAngleInVP
	   >> maxRandomTargetInitialAngleInVP
	   >> minRandomTargetInitialSpeed
	   >> maxRandomTargetInitialSpeed
	   >> minRandomProjectileInitialSpeed
	   >> maxRandomProjectileInitialSpeed
	   >> minRandomTargetColor
	   >> maxRandomTargetColor
	   >> minRandomProjectileColor
	   >> maxRandomProjectileColor
	   >> minRandomTargetAcceleration
	   >> maxRandomTargetAcceleration
	   >> minRandomProjectileAcceleration
	   >> maxRandomProjectileAcceleration
	   >> backgroundColor
	   >> terrainHeight
	   >> terrainColor
	   >> minRandomObstaclePosition
	   >> maxRandomObstaclePosition
	   >> minRandomObstacleRadius
	   >> maxRandomObstacleRadius
	   >> minRandomObstacleColor
	   >> maxRandomObstacleColor;

	in.close();
}

void Settings::save() {
	std::ofstream out(SETTINGS_FILENAME);

	std::string e = "\n";
	
    out << windowSize << e
	    << isCameraControlEnabled << e
		<< cameraAngleInHP << e
		<< cameraAngleInVP << e
		<< mouseSpeed << e
		<< cameraDirection << e
		<< cameraPosition << e
		<< cameraSpeed << e
		<< cameraFieldOfView << e
		<< nearPlane << e 
		<< farPlane << e
		<< trajectoryAdvancePeriod << e
		<< isPaused << e
		<< trajectoryStepCount << e
		<< trajectoryStep << e
		<< gravity << e
		<< angleComputingEpsilon << e
		<< minRandomTargetPosition << e
		<< maxRandomTargetPosition << e
		<< minRandomProjectilePosition << e
		<< maxRandomProjectilePosition << e
		<< minRandomTargetRadius << e
		<< maxRandomTargetRadius << e
		<< minRandomProjectileRadius << e
		<< maxRandomProjectileRadius << e
		<< minRandomTargetInitialAngleInHP << e
		<< maxRandomTargetInitialAngleInHP << e
		<< minRandomTargetInitialAngleInVP << e
		<< maxRandomTargetInitialAngleInVP << e
		<< minRandomTargetInitialSpeed << e
		<< maxRandomTargetInitialSpeed << e
		<< minRandomProjectileInitialSpeed << e 
		<< maxRandomProjectileInitialSpeed << e
		<< minRandomTargetColor << e
		<< maxRandomTargetColor << e
		<< minRandomProjectileColor << e
		<< maxRandomProjectileColor << e
		<< minRandomTargetAcceleration << e
		<< maxRandomTargetAcceleration << e
		<< minRandomProjectileAcceleration << e
		<< maxRandomProjectileAcceleration << e
		<< backgroundColor << e
		<< terrainHeight << e
		<< terrainColor << e
		<< minRandomObstaclePosition << e
		<< maxRandomObstaclePosition << e
		<< minRandomObstacleRadius << e
		<< maxRandomObstacleRadius << e
		<< minRandomObstacleColor << e
		<< maxRandomObstacleColor << e;

	out.close();
}
