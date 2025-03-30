#include "GameApp.h"

#include "imgui.h"
#include "ImGuiLayers.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_opengl3_loader.h"
#include "backends/imgui_impl_sdl3.h"
#include "glm/vec2.hpp"
#include "spdlog/fmt/bundled/format.h"


GameApp& GameApp::Get() {
	static GameApp self;
	return self;
}

std::optional<std::string> GameApp::Init(InitParams params) {
	auto& self = Get();
	return self.InitImpl(std::move(params));
}

GameApp& GameApp::Destroy() {
	auto& self = Get();
	self.DestroyImpl();
	return self;
}

SDL_AppResult GameApp::HandleSDLEvents(void* appstate, SDL_Event& event) {
	ImGui_ImplSDL3_ProcessEvent(&event);
	for (ISDLEventHandler* handler: _eventHandles) {
		handler->HandleEvent(event);
	}

	if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
		RequestQuit();

	return _bQuitRequested ? SDL_APP_SUCCESS : SDL_APP_CONTINUE;
}

SDL_AppResult GameApp::IterateSDL(void* appstate) {
	if (SDL_GetWindowFlags(_window) & SDL_WINDOW_MINIMIZED) {
		SDL_Delay(10);
		return SDL_APP_CONTINUE;
	}
	glm::ivec2 windowSize;
	SDL_GetWindowSize(_window, &windowSize.x, &windowSize.y);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	ImGuiLayers::Get().Render();
	ImGui::Render();

	glViewport(0, 0, windowSize.x, windowSize.x);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	// ---- Opengl drawings -----

	// ~~~~ Opengl drawings ~~~~~

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(_window);
	return SDL_APP_CONTINUE;
}

void GameApp::RequestQuit() {
	_bQuitRequested = true;
}

void GameApp::RegisterEventHandler(ISDLEventHandler* handler) {
	_eventHandles.push_back(handler);
}

void GameApp::UnregisterEventHandler(ISDLEventHandler* handler) {
	if (_bQuitRequested)
		return;

	const auto it = std::ranges::find(_eventHandles, handler);
	if (it != _eventHandles.end()) {
		_eventHandles.erase(it);
	}
}

std::optional<std::string> GameApp::InitImGui() const {
	constexpr const char* glslVersion = "#version 130";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;// Enable Keyboard Controls
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL3_InitForOpenGL(_window, _glContext);
	ImGui_ImplOpenGL3_Init(glslVersion);

	return std::nullopt;
}

std::optional<std::string> GameApp::OpenglAttrsInit() const {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);// Always required on Mac
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	return std::nullopt;
}

std::optional<std::string> GameApp::InitImpl(InitParams&& params) {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		return "Cannot init sdl";
	}

	if (auto error = OpenglAttrsInit()) {
		return error;
	}

	_window = SDL_CreateWindow("Tiny Game", params.width, params.height, SDL_WINDOW_OPENGL);
	if (!_window) {
		return "Cannot create window";
	}

	_glContext = SDL_GL_CreateContext(_window);
	if (_glContext == nullptr) {
		return fmt::format("Error: SDL_GL_CreateContext(): {}\n", SDL_GetError());
	}

	SDL_GL_MakeCurrent(_window, _glContext);

	if (const std::optional<std::string> error = InitImGui()) {
		return *error;
	}

	_initParams = params;

	return std::nullopt;
}

void GameApp::DestroyImpl() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DestroyContext(_glContext);
	_glContext = nullptr;

	SDL_DestroyWindow(_window);
	_window = nullptr;

	SDL_Quit();
}