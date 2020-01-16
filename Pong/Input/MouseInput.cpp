#include "MouseInput.h"

MouseInput MouseInput::Instance;


void MouseInput::BeginNewFrame()
{
	pressedKeys.clear();
	releasedKeys.clear();
}

void MouseInput::MouseDownEvent(const SDL_Event& event)
{
	MouseButtonState state = GetState(event);

	pressedKeys[state] = true;
	heldKeys[state] = true;
}

void MouseInput::MouseUpEvent(const SDL_Event& event)
{
	MouseButtonState state = GetState(event);

	releasedKeys[state] = true;
	heldKeys[state] = false;
}

bool MouseInput::WasMousePressed(const MouseButtonState& key)
{
	return pressedKeys[key];
}

bool MouseInput::WasMouseReleased(const MouseButtonState& key)
{
	return releasedKeys[key];
}

bool MouseInput::IsMouseHeld(const MouseButtonState& key)
{
	return heldKeys[key];
}

MouseButtonState MouseInput::GetState(const SDL_Event& event) const
{
	MouseButtonState state = MouseButtonState::MouseLeft;
	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		{
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				state = MouseButtonState::MouseLeft;
			}
			else if (event.button.button == SDL_BUTTON_RIGHT)
			{
				state = MouseButtonState::MouseRight;
			}
			else if (event.button.button == SDL_BUTTON_MIDDLE)
			{
				state = MouseButtonState::MouseMiddle;
			}
			break;
		}
	case SDL_MOUSEBUTTONUP:
		{
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				state = MouseButtonState::MouseLeft;
			}
			else if (event.button.button == SDL_BUTTON_RIGHT)
			{
				state = MouseButtonState::MouseRight;
			}
			else if (event.button.button == SDL_BUTTON_MIDDLE)
			{
				state = MouseButtonState::MouseMiddle;
			}
			break;
		}
	}
	return state;
}
