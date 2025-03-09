#pragma once
#include <functional>
#include <SDL3/SDL_events.h>

#include "Utils/NonCopyable.h"


class IEventHandler: NonMoveCopyable {
public:
	IEventHandler() = default;
	virtual ~IEventHandler() = default;

	virtual void Handle(const SDL_Event& event) = 0;
};


class CallbackEventHandle: public IEventHandler {
public:
	explicit CallbackEventHandle(std::function<void(const SDL_Event&)>&& callback);

	void Handle(const SDL_Event& event) override;


private:
	std::function<void(const SDL_Event&)> _callback;
};