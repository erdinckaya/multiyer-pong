#include "KeyInput.h"
#include "../Util/Constants.h"
#include "../World/EntityFactory.h"

KeyInput KeyInput::Instance;


void KeyInput::BeginNewFrame()
{
	pressedKeys.clear();
	releasedKeys.clear();
}

void KeyInput::KeyDownEvent(SDL_Event& event)
{
	pressedKeys[event.key.keysym.scancode] = true;
	heldKeys[event.key.keysym.scancode] = true;
}

void KeyInput::KeyUpEvent(SDL_Event& event)
{
	releasedKeys[event.key.keysym.scancode] = true;
	heldKeys[event.key.keysym.scancode] = false;
}

bool KeyInput::WasKeyPressed(SDL_Scancode key)
{
	return pressedKeys[key];
}

bool KeyInput::WasKeyReleased(SDL_Scancode key)
{
	return releasedKeys[key];
}

bool KeyInput::IsKeyHeld(SDL_Scancode key)
{
	return heldKeys[key];
}
