#define SDL_MAIN_USE_CALLBACKS
#include "SDL3/SDL_main.h"
#include "spdlog/spdlog.h"

#include "Graphics/GameApp.h"
#include "Utils/ArgsParser.h"


SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
	ArgsParser::Get().Init(argc, argv);

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		spdlog::error("Cannot init sdl");
		return SDL_APP_FAILURE;
	}
	if (auto error = GameApp::Init(GameApp::InitParams())) {
		spdlog::error("GameApp::Init failed: {0}", *error);
		return SDL_APP_FAILURE;
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
	return GameApp::Get().HandleSDLEvents(appstate, *event);
}

SDL_AppResult SDL_AppIterate(void* appstate) {
	return GameApp::Get().IterageSDL(appstate);
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
	GameApp::Destroy();
	SDL_Quit();
}