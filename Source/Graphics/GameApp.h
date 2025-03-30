#pragma once
#include <optional>
#include <string>

#include <SDL3/SDL.h>

#include "Events/ISDLEventHandler.h"
#include "Utils/NonCopyable.h"


class GameApp: public NonMoveCopyable {
public:
	struct InitParams {
		int16_t width = 1280;
		int16_t height = 720;
	};

	static GameApp& Get();
	static std::optional<std::string> Init(InitParams params);
	static GameApp& Destroy();

	SDL_AppResult HandleSDLEvents(void* appstate, SDL_Event& event);
	SDL_AppResult IterateSDL(void* appstate);

	void RequestQuit();

	void RegisterEventHandler(ISDLEventHandler* handler);
	void UnregisterEventHandler(ISDLEventHandler* handler);


protected:
	std::optional<std::string> InitImGui() const;
	std::optional<std::string> OpenglAttrsInit() const;
	std::optional<std::string> InitImpl(InitParams&& params);
	void DestroyImpl();


private:
	SDL_Window* _window = nullptr;
	SDL_GLContext _glContext = nullptr;
	std::optional<InitParams> _initParams;
	std::vector<ISDLEventHandler*> _eventHandles;

	bool _bQuitRequested = false;
};