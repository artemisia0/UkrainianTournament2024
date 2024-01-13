#include <App.hpp>
#include <Event.hpp>
#include <iostream>
#include <deque>
#include <Util.hpp>
#include <thread>
#include <chrono>


static std::deque<Event> gEventQueue;

App::App()
	: AppElement(nullptr)
	, mTimeSinceLastUpdate(0.0) {
	mContext = defaultContext();
	setContext(mContext.get());
}

App::~App() {
	mSettings->save();
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void App::run() {
	const double timePerUpdate = 1.0/60.0;  // 60 FPS should be OK.
	glfwPollEvents();
	while (not glfwWindowShouldClose(mWindow)
		   and glfwGetKey(mWindow, GLFW_KEY_Q) != GLFW_PRESS) {
		double elapsedTime = glfwGetTime();
		glfwSetTime(0.0);
		mTimeSinceLastUpdate += elapsedTime;
		while (mTimeSinceLastUpdate >= timePerUpdate) {
			mTimeSinceLastUpdate -= timePerUpdate;
			handleEvents();
			update(timePerUpdate);
		}
		draw(elapsedTime);
	}
}

void App::handleEvents() {
	glfwPollEvents();
	while (gEventQueue.size()) {
		auto event = gEventQueue.front();
		gEventQueue.pop_front();
		handleEvent(event);
	}
}

void App::handleEvent(const Event& event) {
	mRenderer->handleEvent(event);
	mScene->handleEvent(event);
}

void App::update(double dt) {
	glCheckError();
	mRenderer->update(dt);
	mScene->update(dt);
	glCheckError();
}

void App::draw(double dt) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	mRenderer->newFrame();

	mScene->draw(dt);

	ImGui::Render();
	int displayW, displayH;
	glfwGetFramebufferSize(mWindow, &displayW, &displayH);
	glViewport(0, 0, displayW, displayH);
	mRenderer->draw(dt);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapInterval(1);
	glfwSwapBuffers(mWindow);
}

std::unique_ptr<Context> App::defaultContext() {
	auto cntx = std::make_unique<Context>();
	cntx->app = this;
	setContext(cntx.get());
	mSettings = defaultSettings();
	cntx->settings = mSettings.get();
	setContext(cntx.get());
	mWindow = defaultWindow();
	assert(mWindow);
	cntx->window = mWindow;
	setContext(cntx.get());
	mRenderer = defaultRenderer();
	cntx->renderer = mRenderer.get();
	setContext(cntx.get());
	mScene = defaultScene();
	cntx->scene = mScene.get();
	setContext(cntx.get());

	return cntx;
}

void __attribute__((__stdcall__)) messageCallback(unsigned,
					 unsigned type,
					 unsigned,
					 unsigned severity,
					 int,
					 const GLchar* message,
					 const void*)
{
	return;

    fprintf(stderr,
			"GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : " "),
			type, severity, message);
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

static void keyCallback(GLFWwindow* window,
						int key, int scancode, int action, int mods) {
    gEventQueue.push_back(Event{window, key, scancode, action, mods});
}

static void glfwScrollCallback(GLFWwindow* window, double dx, double dy) {
    if (dx or dy) {
        gEventQueue.push_back(Event{window, 0, 0, 0, 0, dx, dy});
	}
}

GLFWwindow* App::defaultWindow() {
	assert(context());
	assert(context()->settings);
	
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		fflush(stderr);
		return nullptr;
	}
	
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = nullptr;
	window = glfwCreateWindow(
		context()->settings->windowSize.x,
		context()->settings->windowSize.y,
		"tournament",
		NULL,
		NULL);
	if (window == NULL) {
		fprintf( stderr, "Failed to create GLFW window.\n" );
		fflush(stderr);
		return nullptr;
	}
    glfwMakeContextCurrent(window);
	
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		fflush(stderr);
		return nullptr;
	}
	
	glfwSwapInterval(1);  // Enable vsync which is cool
	
	glfwSetErrorCallback(glfw_error_callback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetScrollCallback(window, glfwScrollCallback);
	
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	
    ImFontConfig font_config;
    font_config.OversampleH = 1;
    font_config.OversampleV = 1;
    font_config.PixelSnapH = 1;
	
    ImFontGlyphRangesBuilder builder;
    static const ImWchar ranges[] = 
    {
        0x0001, 0xffff,
        0,
    };
	
    ImFont* font = io.Fonts->AddFontFromFileTTF(
        "assets/fonts/DejaVuSans.ttf", 20.f, &font_config, ranges);
	assert(font);
	
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);
	
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window,
					 GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    // glfwSetInputMode(g_context->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(messageCallback, nullptr);
	
	if (not window) {
		std::cerr << "Failed to create GLFW window" << std::endl;
	}

	glfwSwapInterval(1);
	
	return window;
}

std::unique_ptr<Settings> App::defaultSettings() {
	auto settings = std::make_unique<Settings>();
	// Default Settings class constructor is OK.

	if (settings->canBeLoaded()) {
		settings->load();
	}
	
	return settings;
}

std::unique_ptr<Scene> App::defaultScene() {
	assert(context());
	auto scene = std::make_unique<Scene>(context());
	return scene;
}

std::unique_ptr<Renderer> App::defaultRenderer() {
	assert(context());
	auto renderer = std::make_unique<Renderer>(context());
	return renderer;
}

void App::resetSettings() {
	mSettings = std::make_unique<Settings>();
	mContext->settings = mSettings.get();
}
