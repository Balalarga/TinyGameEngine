#pragma once
#include <functional>
#include <SDL3/SDL_events.h>


class ISDLEventHandler {
public:
	ISDLEventHandler();
	ISDLEventHandler(ISDLEventHandler&&) = default;
	ISDLEventHandler& operator=(ISDLEventHandler&&) = default;
	virtual ~ISDLEventHandler();

	virtual void HandleEvent(const SDL_Event& event) = 0;


protected:
	ISDLEventHandler(const ISDLEventHandler&) = default;
	ISDLEventHandler& operator=(const ISDLEventHandler&) = default;
};


class SDLCallbackEventHandle: public ISDLEventHandler {
public:
	explicit SDLCallbackEventHandle(std::function<void(const SDL_Event&)>&& callback);

	void HandleEvent(const SDL_Event& event) override;


private:
	std::function<void(const SDL_Event&)> _callback;
};