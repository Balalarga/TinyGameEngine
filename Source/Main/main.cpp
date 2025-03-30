#define SDL_MAIN_USE_CALLBACKS
#include "imgui.h"
#include "Events/InputSystem.h"
#include "SDL3/SDL_main.h"
#include "spdlog/spdlog.h"

#include "Graphics/GameApp.h"
#include "Graphics/ImGuiLayers.h"
#include "Utils/ArgsParser.h"


SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
	ArgsParser::Get().Init(argc, argv);

	if (auto error = GameApp::Init(GameApp::InitParams())) {
		spdlog::error("GameApp::Init failed: {0}", *error);
		return SDL_APP_FAILURE;
	}

	static auto imguiDemoWindow = std::make_shared<ImGuiCallbackLayer>(
		[]() {
			ImGui::ShowDemoWindow();
		});
	ImGuiLayers::PushLayer(imguiDemoWindow);

	InputSystem::BindKey(
		SDL_SCANCODE_ESCAPE,
		[]() {
			GameApp::Get().RequestQuit();
		});

	InputSystem::BindKey(
		SDL_SCANCODE_Q,
		[]() {
			imguiDemoWindow.reset();
		});

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
	return GameApp::Get().HandleSDLEvents(appstate, *event);
}

SDL_AppResult SDL_AppIterate(void* appstate) {
	return GameApp::Get().IterateSDL(appstate);
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
	GameApp::Destroy();
	SDL_Quit();
}