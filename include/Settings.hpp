#pragma once

#include <gl.hpp>


class Settings {
public:
	bool canBeLoaded();
	void load();
	void save();


public:
	ivec2 windowSize = ivec2(1920, 1080);
	bool isCameraControlEnabled = false;
	float cameraAngleInHP = 3.14f;
	float cameraAngleInVP = 0.f;
	float mouseSpeed = 0.004f;
	vec3 cameraDirection = vec3(0, 0, -1);
	vec3 cameraPosition = vec3(0, 0, 3);
	float cameraSpeed = 50.f;
	float cameraFieldOfView = 45.f;
	float nearPlane = 0.1f;
	float farPlane = 800.f;
	float trajectoryAdvancePeriod = 0.08;
	bool isPaused = true;
	int trajectoryStepCount = 300;
	float trajectoryStep = 0.02;
    float gravity = -9.78f;
	float angleComputingEpsilon = 0.016;
	vec3 minRandomTargetPosition = vec3(-100, -100, -100);
	vec3 maxRandomTargetPosition = vec3(100, 100, 100);
	vec3 minRandomProjectilePosition = vec3(-50, -50, -50);
	vec3 maxRandomProjectilePosition = vec3(50, 50, 50);
	float minRandomTargetRadius = 1;
	float maxRandomTargetRadius = 4;
	float minRandomProjectileRadius = 2;
	float maxRandomProjectileRadius = 5;
	float minRandomTargetInitialAngleInHP = 0;
	float maxRandomTargetInitialAngleInHP = 3.14*2;
	float minRandomTargetInitialAngleInVP = 0;
	float maxRandomTargetInitialAngleInVP = 3.14*2;
	float minRandomTargetInitialSpeed = 1;
	float maxRandomTargetInitialSpeed = 10;
	float minRandomProjectileInitialSpeed = 50;
	float maxRandomProjectileInitialSpeed = 100;
	vec3 minRandomTargetColor = vec3(0.3, 0.3, 0.3);
	vec3 maxRandomTargetColor = vec3(0.6, 0.6, 0.6);
	vec3 minRandomProjectileColor = vec3(0.7, 0.7, 0.7);
	vec3 maxRandomProjectileColor = vec3(1, 1, 1);
	float minRandomTargetAcceleration = 0;
	float maxRandomTargetAcceleration = 5;
	float minRandomProjectileAcceleration = 2;
	float maxRandomProjectileAcceleration = 20;
	vec3 backgroundColor = vec3(0.1, 0.1, 0.1);
	float terrainHeight = -100;
	vec3 terrainColor = vec3(0.3, 0.3, 0.3);
	vec3 minRandomObstaclePosition = vec3(-100, -100, -100);
	vec3 maxRandomObstaclePosition = vec3(100, 100, 100);
	float minRandomObstacleRadius = 3;
	float maxRandomObstacleRadius = 15;
	vec3 minRandomObstacleColor = vec3(0.4, 0.4, 0.4);
	vec3 maxRandomObstacleColor = vec3(0.9, 0.9, 0.9);
};
