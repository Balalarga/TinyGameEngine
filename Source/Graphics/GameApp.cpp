#include "GameApp.h"


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
	if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
		return SDL_APP_SUCCESS;
	}

	for (IEventHandler& handler: _eventHandles) {
		handler.Handle(event);
	}

	if (event.type == SDL_EVENT_KEY_DOWN) {
		if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
			return SDL_APP_SUCCESS;
		}
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult GameApp::IterageSDL(void* appstate) {
	SDL_RenderClear(_renderer);

	SDL_RenderPresent(_renderer);
	return SDL_APP_CONTINUE;
}

std::optional<std::string> GameApp::InitImpl(InitParams&& params) {
	_window = SDL_CreateWindow("Tiny Game", params.width, params.height, SDL_WINDOW_VULKAN);
	if (!_window) {
		return "Cannot create window";
	}

	_renderer = SDL_CreateRenderer(_window, nullptr);
	if (!_renderer) {
		return "Cannot create renderer";
	}

	_initParams = params;

	return std::nullopt;
}

void GameApp::DestroyImpl() {
	SDL_DestroyRenderer(_renderer);
	_renderer = nullptr;

	SDL_DestroyWindow(_window);
	_window = nullptr;
}