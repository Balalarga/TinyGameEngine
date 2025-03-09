#include "IEventHandler.h"


CallbackEventHandle::CallbackEventHandle(std::function<void(const SDL_Event&)>&& callback): _callback(callback) {
}

void CallbackEventHandle::Handle(const SDL_Event& event) {
	if (_callback)
		_callback(event);
}