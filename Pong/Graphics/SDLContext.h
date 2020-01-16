#pragma once


#include <memory>
#include "SDLWindow.h"

/// Creates SDL contexts
class SDLContext
{
public:
	SDLContext(SDLWindow& sdlWindow);

	operator SDL_GLContext() const;
private:
	std::unique_ptr<void, void(*)(SDL_GLContext)> context;
};
