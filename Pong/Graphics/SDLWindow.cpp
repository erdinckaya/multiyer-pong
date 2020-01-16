#include "SDLWindow.h"
#include "../Util/UtilMacro.h"

/// Initializes SDL2 window with flags.
SDLWindow::SDLWindow(unsigned int width, unsigned int height, const char* title, int flags)
	: window(SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
	                          flags), static_cast<void (&&)(SDL_Window*)>(SDL_DestroyWindow))
{
	CHECK_ERROR_EXIT(window.get() == nullptr, SDL_GetError());
}


/// Fetches window's raw pointer.
SDLWindow::operator SDL_Window *() const
{
	return window.get();
}

/// Shows window
void SDLWindow::Show() const
{
	SDL_ShowWindow(window.get());
}

/// Sets window title.
void SDLWindow::SetWindowTitle(const char* title) const
{
	SDL_SetWindowTitle(window.get(), title);
}

/// Sets window direction if you have two monitors it opens itself into second monitor.
void SDLWindow::SetWindowDirection(const Direction& direction) const
{
	const int displays = SDL_GetNumVideoDisplays();
	int display = 0;
	if (displays > 1)
	{
		display = 1;
	}
	SDL_Rect displayBound;
	SDL_GetDisplayBounds(display, &displayBound);
	int wW;
	int wH;
	SDL_GetWindowSize(window.get(), &wW, &wH);
	DEBUG_LOG("Display bound x {0} w {1}", displayBound.x, displayBound.w);
	DEBUG_LOG("Window size w {0} h {1}", wW, wH);
	float x;
	float y;
	switch (direction)
	{
	case Direction::Left:
		{
			x = displayBound.x + 10;
			y = 50;
			break;
		}
	case Direction::Right:
		{
			x = displayBound.x + displayBound.w - wW - 10;
			y = 50;
			break;
		}
	}


	SDL_SetWindowPosition(window.get(), x, y);
}
