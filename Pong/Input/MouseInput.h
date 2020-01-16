#pragma once
#include <SDL2/SDL.h>
#include <unordered_map>
#include "../Util/Types.h"

/// Mouse input manager that handles all mouse events
/// that polled from SDL2
class MouseInput
{
public:
	static MouseInput Instance;

	void BeginNewFrame();

	void MouseUpEvent(const SDL_Event& event);

	void MouseDownEvent(const SDL_Event& event);

	bool WasMousePressed(const MouseButtonState& key);

	bool WasMouseReleased(const MouseButtonState& key);

	bool IsMouseHeld(const MouseButtonState& key);

private:
	MouseButtonState GetState(const SDL_Event& event) const;

	std::unordered_map<int, bool> heldKeys;
	std::unordered_map<int, bool> pressedKeys;
	std::unordered_map<int, bool> releasedKeys;
};
