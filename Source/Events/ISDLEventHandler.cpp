#include "ISDLEventHandler.h"

#include "Graphics/GameApp.h"


ISDLEventHandler::ISDLEventHandler() {
	GameApp::Get().RegisterEventHandler(this);
}

ISDLEventHandler::~ISDLEventHandler() {
	GameApp::Get().UnregisterEventHandler(this);
}

SDLCallbackEventHandle::SDLCallbackEventHandle(std::function<void(const SDL_Event&)>&& callback): _callback(callback) {
}

void SDLCallbackEventHandle::HandleEvent(const SDL_Event& event) {
	if (_callback)
		_callback(event);
}