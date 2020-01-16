#pragma once

#include <SDL2/SDL_surface.h>
#include <memory>

/// Keeps sdl surfaces and handles memory managements as well.
class SDLSurface
{
public:
	SDLSurface(const char* filename);

	SDLSurface(SDL_Surface* surface);

	operator SDL_Surface *() const;

	void Bind() const;

	int Width() const;

	int Height() const;

private:
	static void DeleteTexture(unsigned int* textureId);

	std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)> surface;
	std::unique_ptr<unsigned int, void(*)(unsigned int*)> textureId;
};
