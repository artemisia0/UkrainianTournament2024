#include <GUI.hpp>
#include <imgui.hpp>
#include <Settings.hpp>
#include <Context.hpp>
#include <Scene.hpp>
#include <Util.hpp>
#include <Projectile.hpp>
#include <Target.hpp>
#include <Model.hpp>
#include <iostream>
#include <App.hpp>
#include <Obstacle.hpp>
#include <fstream>


GUI::GUI(Context* cntx)
	: AppElement(cntx)
	, mTranslator(nullptr)
	, mIsOptionsMenuOpened(false)
	, mIsHelpPageOpened(false)
	, mLanguage(ENGLISH_LANGUAGE)
	, mTheme(1) {
	mTranslator = std::make_unique<Translator>();
	mTranslator->loadTranslation();

	std::ifstream in("./translation/help_eng.txt");
	std::getline(in, mHelpText[ENGLISH_LANGUAGE], '\0');
	in.close();

	in = std::ifstream("./translation/help_ukr.txt");
	std::getline(in, mHelpText[UKRAINIAN_LANGUAGE], '\0');
	in.close();
}

void GUI::drawEditor(Noise& noise) {
	drawEditor(noise.mix, tr("Mix"));
	drawEditor(noise.frequency, tr("Frequency"));
	drawEditor(noise.seed, tr("Seed"));
}

void GUI::draw(double) {
	if (mTheme == 0) {
		ImGui::StyleColorsDark();
	} else if (mTheme == 1) {
		ImGui::StyleColorsClassic();
	} else if (mTheme == 2) {
		ImGui::StyleColorsLight();
	} else {
		std::cerr << "GUI::draw: Invalid ImGui theme" << std::endl;
	}
	
	drawMainMenuBar();
	drawOpenedWindows();
}

void GUI::drawOpenedWindows() {
	if (mIsOptionsMenuOpened) {
		drawOptionsMenu();
	}
	if (mIsHelpPageOpened) {
		drawHelpPage();
	}
	for (const auto& p : mIsPairEditorOpened) {
		if (p.second) {
			drawPairEditor(p.first);
		}
	}
	for (const auto& p : mIsObstacleEditorOpened) {
		if (p.second) {
			drawObstacleEditor(p.first);
		}
	}
}

void GUI::drawBodyEditor(Body* body, int) {
	ImGui::ColorEdit3(tr("Body color").c_str(), (float*)(&body->color));
	drawEditor(body->initialSpeed, tr("Speed"));
	drawEditor(body->acceleration, tr("Acceleration"));
	drawEditor(body->radius, tr("Radius"));
	if (body->trajectory->size()) {
		drawEditor(body->trajectory->at(0), tr("Position"));
	} else {
		drawEditor(body->position, tr("Position"));
	}

	auto pointsString = tr("Points");
	auto linesString = tr("Lines");
	auto trianglesString = tr("Triangles");
	auto noneString = tr("None");
	const char* drawingModes[] {
		pointsString.c_str(),
		linesString.c_str(),
		trianglesString.c_str(),
		noneString.c_str(),
	};
	ImGui::Combo(tr("Drawing mode").c_str(), &body->drawingMode,
				 drawingModes, IM_ARRAYSIZE(drawingModes));
	drawEditor(body->toResetTrajectory,
		tr("To reset trajectory"));
}

void GUI::drawEditor(Trajectory& trajectory) {
	ImGui::ColorEdit3(tr("Color").c_str(), (float*)&trajectory.color);
	for (int i = 0; i < int(trajectory.size()); ++i) {
		drawTrajectoryVertexEditor(trajectory.at(i), i);
	}
}

void GUI::show(Trajectory& trajectory) {
	ImGui::ColorEdit3(tr("Color").c_str(), (float*)&trajectory.color);
	for (int i = 0; i < int(trajectory.size()); ++i) {
		vec3 v = trajectory.at(i);
		drawTrajectoryVertexEditor(v, i);
	}
}

void GUI::drawTrajectoryVertexEditor(vec3& v, int i) {
	drawEditor(v, std::string(tr("Vertex #") + std::to_string(i)).c_str());
}

void GUI::drawMainMenuBar() {
	ImGui::BeginMainMenuBar();
	ImGui::Checkbox(tr("Options").c_str(), &mIsOptionsMenuOpened);
	ImGui::Checkbox(tr("Help").c_str(), &mIsHelpPageOpened);
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text(tr("Projectile/Target pair count: %d").c_str(),
				int(context()->scene->pairs().size()));
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text(tr("Obstacles count: %d").c_str(),
				int(context()->scene->obstacles().size()));
	int destroyedTargets = 0;
	for (auto& p : context()->scene->pairs()) {
		if (p.target()->isDestroyed) {
			++destroyedTargets;
		}
	}
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text(tr("Destroyed targets count: %d").c_str(), destroyedTargets);
	ImGui::EndMainMenuBar();
}

void GUI::drawPairEditor(int index) {
	ImGui::Begin(
		std::string(tr("Body pair editor #")
					+ std::to_string(index)).c_str(),
		&mIsPairEditorOpened[index]);

	if (ImGui::Button(tr("Remove this pair").c_str())) {
		mIsPairEditorOpened[index] = false;
		context()->scene->pairs().at(index).remove();
	}

	ImGui::BeginTabBar(std::string("drawPairEditor"
								   + std::to_string(index)).c_str());
	if (ImGui::BeginTabItem(
			std::string(tr("Target #")
						+ std::to_string(index)).c_str())) {
		Target* target = context()->scene->pairs().at(index).target();
		drawBodyEditor(target, index);
		drawEditor(target->initialAngleInHP, tr("Angle in horizontal plane"));
		drawEditor(target->initialAngleInVP, tr("Angle in vertical plane"));
		drawEditor(mIsEditTargetNoiseOpened[index], tr("Edit noise"));
		if (mIsEditTargetNoiseOpened[index]) {
			drawEditor(target->noise);
		}
		drawEditor(mIsEditTargetTrajectoryOpened[index],
				   tr("Edit trajectory"));
		if (mIsEditTargetTrajectoryOpened[index]) {
			drawEditor(*target->trajectory);
		}
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem(
			std::string(tr("Projectile #")
						+ std::to_string(index).c_str()).c_str())) {
		Projectile* projectile =
			context()->scene->pairs().at(index).projectile();
		drawBodyEditor(
			context()->scene->pairs().at(index).projectile(), index);
		drawEditor(projectile->toDrawModel, tr("Draw model"));
		drawEditor(*projectile->model(), tr("Model"));
		ImGui::Text(tr("Angle in horizontal plane: %.4f").c_str(),
					projectile->initialAngleInHP);
		ImGui::Text(tr("Angle in vertical plane: %.4f").c_str(),
					projectile->initialAngleInVP);
		drawEditor(mIsShowProjectileTrajectoryOpened[index],
				   tr("Show trajectory"));
		if (mIsShowProjectileTrajectoryOpened[index]) {
			show(*projectile->trajectory);
		}
		ImGui::EndTabItem();
	}
									
	ImGui::EndTabBar();
	ImGui::End();
}

void GUI::drawObstacleEditor(int index) {
	ImGui::Begin(
		std::string(tr("Obstacle editor #")
					+ std::to_string(index)).c_str(),
		&mIsObstacleEditorOpened[index]);
	if (ImGui::Button(tr("Remove this obstacle").c_str())) {
		mIsObstacleEditorOpened[index] = false;
		context()->scene->obstacles().at(index)->remove();
	}
	drawEditor(*context()->scene->obstacles().at(index));
	ImGui::End();
}

void GUI::drawEditor(Obstacle& obstacle) {
	drawEditor(obstacle.position, tr("Position"));
	drawEditor(obstacle.radius, tr("Radius"));
	auto pointsString = tr("Points");
	auto linesString = tr("Lines");
	auto trianglesString = tr("Triangles");
	auto noneString = tr("None");
	const char* drawingModes[] {
		pointsString.c_str(),
		linesString.c_str(),
		trianglesString.c_str(),
		noneString.c_str(),
	};
	ImGui::Combo(tr("Drawing mode").c_str(), &obstacle.drawingMode,
				 drawingModes, IM_ARRAYSIZE(drawingModes));
	ImGui::ColorEdit3(tr("Color").c_str(), (float*)&obstacle.color);
}

void GUI::drawEditor(Model& model, const std::string&) {
	drawEditor(model.modelScaling, tr("Model scaling"));
}

void GUI::drawOptionsMenu() {
	ImGui::Begin(tr("Options").c_str(), &mIsOptionsMenuOpened);
	ImGui::BeginTabBar("OptionsTabBar");
	if (ImGui::BeginTabItem(tr("General").c_str())) {
		const char* languages[] = {
			"English",
			"Українська",
		};
		ImGui::Combo(tr("Language").c_str(), &mLanguage, languages,
					 IM_ARRAYSIZE(languages));
		drawEditor(context()->settings->windowSize, tr("Window size"));
		drawEditor(context()->settings->trajectoryAdvancePeriod,
				   tr("Simulation step period"));
		drawEditor(context()->settings->trajectoryStepCount,
				   tr("Trajectory length"));
		drawEditor(context()->settings->trajectoryStep,
				   tr("Trajectory step"));
		drawEditor(context()->settings->gravity,
				   tr("Gravity"));
		drawEditor(context()->settings->angleComputingEpsilon,
				   tr("Projectile angles epsilon"));
		ImGui::ColorEdit3(tr("Background").c_str(),
			(float*)&context()->settings->backgroundColor);
		auto darkString = tr("Dark");
		auto grayString = tr("Gray");
		auto lightString = tr("Light");
		const char* themes[] = {
			darkString.c_str(),
			grayString.c_str(),
			lightString.c_str(),
		};
		ImGui::ColorEdit3(tr("Grid color").c_str(),
						  (float*)&context()->settings->terrainColor);
		drawEditor(context()->settings->terrainHeight, tr("Grid height"));
		ImGui::Combo(tr("Color theme").c_str(), &mTheme, themes,
					 IM_ARRAYSIZE(themes));
		if (ImGui::Button(tr("Reset settings").c_str())) {
			resetSettings();
		}
		drawEditor(context()->settings->isPaused, tr("Pause"));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem(tr("Camera").c_str())) {
		drawEditor(context()->settings->isCameraControlEnabled,
				   tr("Camera control enabled"));
		drawEditor(context()->settings->cameraAngleInHP,
				   tr("Camera angle in horizontal plane"));
		drawEditor(context()->settings->cameraAngleInVP,
				   tr("Camera angle in vertical plane"));
		drawEditor(context()->settings->mouseSpeed,
				   tr("Mouse sensitivity"));
		drawEditor(context()->settings->cameraDirection,
				   tr("Camera direction"));
		drawEditor(context()->settings->cameraPosition,
				   tr("Camera position"));
		drawEditor(context()->settings->cameraSpeed,
				   tr("Camera speed"));
		drawEditor(context()->settings->cameraFieldOfView,
				   tr("Field of view"));
		drawEditor(context()->settings->nearPlane,
				   tr("Near plane"));
		drawEditor(context()->settings->farPlane,
				   tr("Far plane"));
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem(tr("Body creating").c_str())) {
		if (ImGui::Button(
				tr("Create random projectile and target").c_str())) {
			createRandomPair();
		}
		if (ImGui::Button(tr("Create random obstacle").c_str())) {
			createRandomObstacle();
		}
		drawEditor(context()->settings->minRandomTargetPosition,
				   tr("Minimum target position"));
		drawEditor(context()->settings->maxRandomTargetPosition,
				   tr("Maximum target position"));
		drawEditor(context()->settings->minRandomProjectilePosition,
				   tr("Minimum projectile position"));
		drawEditor(context()->settings->maxRandomProjectilePosition,
				   tr("Maximum projectile position"));
		drawEditor(context()->settings->minRandomObstaclePosition,
				   tr("Minimum obstacle position"));
		drawEditor(context()->settings->maxRandomObstaclePosition,
				   tr("Maximum obstacle position"));
		drawEditor(context()->settings->minRandomTargetRadius,
				   tr("Minimum target radius"));
		drawEditor(context()->settings->maxRandomTargetRadius,
				   tr("Maximum target radius"));
		drawEditor(context()->settings->minRandomProjectileRadius,
				   tr("Minimum projectile radius"));
		drawEditor(context()->settings->maxRandomProjectileRadius,
				   tr("Maximum projectile radius"));
		drawEditor(context()->settings->minRandomObstacleRadius,
				   tr("Minimum obstacle radius"));
		drawEditor(context()->settings->maxRandomObstacleRadius,
				   tr("Maximum obstacle radius"));
		drawEditor(context()->settings->minRandomTargetInitialAngleInHP,
				   tr("Minimum target angle (horizontal)"));
		drawEditor(context()->settings->maxRandomTargetInitialAngleInHP,
				   tr("Maximum target angle (horizontal)"));
		drawEditor(context()->settings->minRandomTargetInitialAngleInVP,
				   tr("Minimum target angle (vertical)"));
		drawEditor(context()->settings->maxRandomTargetInitialAngleInVP,
				   tr("Maximum target angle (vertical)"));
		drawEditor(context()->settings->minRandomTargetInitialSpeed,
				   tr("Minimum target initial speed"));
		drawEditor(context()->settings->maxRandomTargetInitialSpeed,
				   tr("Maximum target initial speed"));
		drawEditor(context()->settings->minRandomProjectileInitialSpeed,
				   tr("Minimum projectile initial speed"));
		drawEditor(context()->settings->maxRandomProjectileInitialSpeed,
				   tr("Maximum projectile initial speed"));
		drawEditor(context()->settings->minRandomTargetColor,
				   tr("Minimum target color"));
		drawEditor(context()->settings->maxRandomTargetColor,
				   tr("Maximum target color"));
		drawEditor(context()->settings->minRandomProjectileColor,
				   tr("Minimum projectile color"));
		drawEditor(context()->settings->maxRandomProjectileColor,
				   tr("Maximum projectile color"));
		drawEditor(context()->settings->minRandomObstacleColor,
				   tr("Minimum obstacle color"));
		drawEditor(context()->settings->maxRandomObstacleColor,
				   tr("Maximum obstacle color"));
		drawEditor(context()->settings->minRandomTargetAcceleration,
				   tr("Minimum target acceleration"));
		drawEditor(context()->settings->maxRandomTargetAcceleration,
				   tr("Maximum target acceleration"));
		drawEditor(context()->settings->minRandomProjectileAcceleration,
				   tr("Minimum projectile acceleration"));
		drawEditor(context()->settings->maxRandomProjectileAcceleration,
				   tr("Maximum projectile acceleration"));
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
	ImGui::End();
}

void GUI::drawHelpPage() {
	ImGui::Begin(tr("Help").c_str(), &mIsHelpPageOpened);
	ImGui::Text("%s", mHelpText[mLanguage].c_str());
	ImGui::End();
}

std::string GUI::tr(const std::string& s) {
	return mTranslator->translate(s, mLanguage);
}

void GUI::drawEditor(vec3& v, const std::string& s) {
	ImGui::DragFloat3(s.c_str(), (float*)&v);
}

void GUI::drawEditor(ivec2& v, const std::string& s) {
	ImGui::DragInt2(s.c_str(), (int*)&v);
}

void GUI::drawEditor(float& v, const std::string& s) {
	ImGui::DragFloat(s.c_str(), &v);
}

void GUI::drawEditor(int& v, const std::string& s) {
	ImGui::DragInt(s.c_str(), &v);
}

void GUI::drawEditor(bool& v, const std::string& s) {
	ImGui::Checkbox(s.c_str(), &v);
}

void GUI::createRandomPair() {
	auto projectile = std::make_unique<Projectile>(context());
	auto target = std::make_unique<Target>(context());
	setRandomProjectileParameters(projectile.get());
	setRandomTargetParameters(target.get());
	context()->scene->pairs().push_back(
		Pair(
			context(),
			std::move(projectile),
			std::move(target)));
}

void GUI::createRandomObstacle() {
	auto obstacle = std::make_unique<Obstacle>(context());
	setRandomObstacleParameters(obstacle.get());
	context()->scene->obstacles().push_back(std::move(obstacle));
}

void GUI::setRandomProjectileParameters(Projectile* p) {
	p->position = randomVec3(
		context()->settings->minRandomProjectilePosition,
		context()->settings->maxRandomProjectilePosition);
	p->initialSpeed = randomFloat(
		context()->settings->minRandomProjectileInitialSpeed,
		context()->settings->maxRandomProjectileInitialSpeed);
	p->color = randomVec3(
		context()->settings->minRandomProjectileColor,
		context()->settings->maxRandomProjectileColor);
	p->acceleration = randomFloat(
		context()->settings->minRandomProjectileAcceleration,
		context()->settings->maxRandomProjectileAcceleration);
	p->radius = randomFloat(
		context()->settings->minRandomProjectileRadius,
		context()->settings->maxRandomProjectileRadius);
}

void GUI::setRandomTargetParameters(Target* p) {
	p->position = randomVec3(
		context()->settings->minRandomTargetPosition,
		context()->settings->maxRandomTargetPosition);
	p->initialAngleInHP = randomFloat(
		context()->settings->minRandomTargetInitialAngleInHP,
		context()->settings->maxRandomTargetInitialAngleInHP);
	p->initialAngleInVP = randomFloat(
		context()->settings->minRandomTargetInitialAngleInVP,
		context()->settings->maxRandomTargetInitialAngleInVP);
	p->initialSpeed = randomFloat(
		context()->settings->minRandomTargetInitialSpeed,
		context()->settings->maxRandomTargetInitialSpeed);
	p->color = randomVec3(
		context()->settings->minRandomTargetColor,
		context()->settings->maxRandomTargetColor);
	p->acceleration = randomFloat(
		context()->settings->minRandomTargetAcceleration,
		context()->settings->maxRandomTargetAcceleration);
	p->radius = randomFloat(
		context()->settings->minRandomTargetRadius,
		context()->settings->maxRandomTargetRadius);
}

void GUI::setRandomObstacleParameters(Obstacle* o) {
	o->radius = randomFloat(
		context()->settings->minRandomObstacleRadius,
		context()->settings->maxRandomObstacleRadius);
	o->color = randomVec3(
		context()->settings->minRandomObstacleColor,
		context()->settings->maxRandomObstacleColor);
	o->position = randomVec3(
		context()->settings->minRandomObstaclePosition,
		context()->settings->maxRandomObstaclePosition);
}

void GUI::mousePickingPairCallback(int pairIndex) {
	assert(pairIndex >= 0
		   and pairIndex < int(context()->scene->pairs().size()));
	mIsPairEditorOpened[pairIndex] = not mIsPairEditorOpened[pairIndex];
}

void GUI::mousePickingObstacleCallback(int obstacleIndex) {
	assert(obstacleIndex >= 0
		   and obstacleIndex < int(context()->scene->obstacles().size()));
	mIsObstacleEditorOpened[obstacleIndex] =
		not mIsObstacleEditorOpened[obstacleIndex];
}

void GUI::resetSettings() {
	context()->app->resetSettings();
}
