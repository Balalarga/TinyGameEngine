#pragma once
#include <array>
#include <functional>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_scancode.h>

#include "ISDLEventHandler.h"
#include "Utils/NonCopyable.h"


class InputSystem: public NonMoveCopyable, public ISDLEventHandler {
	constexpr static auto MouseKeyCount = SDL_BUTTON_X2 + 1;


public:
	using OnKeyDown = std::function<void()>;
	using OnMouseKeyDown = std::function<void()>;
	using OnMouseMove = std::function<void(float, float)>;


	static InputSystem& Get();

	static void BindKey(SDL_Scancode key, OnKeyDown&& action);
	static void BindMouseKey(SDL_MouseButtonFlags button, OnMouseKeyDown&& action);
	static void BindMouseMove(OnMouseMove&& action);

	void HandleEvent(const SDL_Event& event) override;


private:
	std::array<std::vector<OnKeyDown>, SDL_SCANCODE_COUNT> _onKeyDown;
	std::array<std::vector<OnMouseKeyDown>, MouseKeyCount> _onMouseKeyDown;
	std::vector<OnMouseMove> _onMouseMove;
};