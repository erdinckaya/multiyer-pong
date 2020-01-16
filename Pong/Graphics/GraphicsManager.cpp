#include "GraphicsManager.h"


GraphicsManager GraphicsManager::Instance;

GraphicsManager::GraphicsManager() : ready(false)
{
}

/// Since server is headless we don't need to init graphics in Server configuration.
void GraphicsManager::Init()
{
#ifdef CLIENT
	graphics = std::make_unique<Graphics>(CLIENT_WINDOW_WIDTH, CLIENT_WINDOW_HEIGHT, CLIENT_WINDOW_TITLE, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);
	ready = true;
#else
	ready = false;
#endif
}

/// Gets graphics instance.
Graphics* GraphicsManager::GetGraphics() const
{
	return graphics.get();
}

/// Check graphic instance's availability.
bool GraphicsManager::Ready() const
{
#ifdef SERVER
	return false;
#else
	return ready;
#endif
}
