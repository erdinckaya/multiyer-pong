#pragma once

#include <memory>
#include <SDL2/SDL_video.h>

#include "../Util/Types.h"


/// Creates a simple SDL2 window
class SDLWindow
{
public:
	SDLWindow(unsigned int width, unsigned int height, const char* title, int flags);

	operator SDL_Window *() const;

	void Show() const;

	void SetWindowTitle(const char* title) const;

	void SetWindowDirection(const Direction& direction) const;
private:
	std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window;
};
