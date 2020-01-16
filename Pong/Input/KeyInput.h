#pragma once
#include <SDL2/SDL.h>
#include <unordered_map>

/// Key input manager that handles all keyboard events
/// that polled from SDL2
class KeyInput
{
public:
	static KeyInput Instance;

	void BeginNewFrame();

	void KeyUpEvent(SDL_Event& event);

	void KeyDownEvent(SDL_Event& event);

	bool WasKeyPressed(SDL_Scancode key);

	bool WasKeyReleased(SDL_Scancode key);

	bool IsKeyHeld(SDL_Scancode key);
	
private:
	std::unordered_map<SDL_Scancode, bool> heldKeys;
	std::unordered_map<SDL_Scancode, bool> pressedKeys;
	std::unordered_map<SDL_Scancode, bool> releasedKeys;
};
