#include "InputSystem.h"


InputSystem& InputSystem::Get() {
	static InputSystem instance;
	return instance;
}

void InputSystem::BindKey(SDL_Scancode key, OnKeyDown&& action) {
	auto& self = Get();
	self._onKeyDown[static_cast<int32_t>(key)].push_back(action);
}

void InputSystem::BindMouseKey(SDL_MouseButtonFlags button, OnMouseKeyDown&& action) {
	auto& self = Get();
	self._onMouseKeyDown[static_cast<int32_t>(button)].push_back(action);
}

void InputSystem::BindMouseMove(OnMouseMove&& action) {
	auto& self = Get();
	self._onMouseMove.push_back(action);
}

void InputSystem::HandleEvent(const SDL_Event& event) {
	const bool isKeyEvent = event.type == SDL_EVENT_KEY_UP || event.type == SDL_EVENT_KEY_DOWN;
	const bool isMouseKeyEvent = event.type == SDL_EVENT_MOUSE_BUTTON_UP || event.type == SDL_EVENT_MOUSE_BUTTON_DOWN;
	const bool isMouseMoveEvent = event.type == SDL_EVENT_MOUSE_MOTION;

	if (isKeyEvent) {
		for (auto& action: _onKeyDown[event.key.scancode]) {
			action();
		}
	}

	if (isMouseKeyEvent) {
		for (auto& action: _onMouseKeyDown[event.button.button]) {
			action();
		}
	}

	if (isMouseMoveEvent) {
		for (auto& action: _onMouseMove) {
			action(event.motion.xrel, event.motion.yrel);
		}
	}
}