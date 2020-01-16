#include "SDLSurface.h"

#include "../Util/UtilMacro.h"

#include <SDL2/SDL_image.h>
#include <GL/glew.h>

SDLSurface::SDLSurface(const char* filename) : SDLSurface(IMG_Load(filename))
{
}

/// Initializes sdl surface and stores it.
SDLSurface::SDLSurface(SDL_Surface* surface)
	: surface(surface, static_cast<void (&&)(SDL_Surface*)>(SDL_FreeSurface)),
	  textureId(new unsigned int, &DeleteTexture)
{
	CHECK_ERROR_EXIT(surface == nullptr, IMG_GetError());
	
	glGenTextures(1, textureId.get());
	glBindTexture(GL_TEXTURE_2D, *textureId);
	GLenum mode;
	switch (surface->format->BytesPerPixel)
	{
	case 4:
		mode = GL_RGBA;
		break;
	case 3:
		mode = GL_RGB;
		break;
	case 1:
		mode = GL_LUMINANCE_ALPHA;
		break;
	default:
		LOG_ERROR("Image with unknown channel profile");
		break;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

/// Deletes texture.
void SDLSurface::DeleteTexture(unsigned int* textureId)
{
	glDeleteTextures(1, textureId);
}

/// Binds texture to opengl
void SDLSurface::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, *textureId);
}

/// Gets real width of surface.
int SDLSurface::Width() const
{
	return surface->w;
}

/// Gets real height of surface.
int SDLSurface::Height() const
{
	return surface->h;
}

SDLSurface::operator SDL_Surface *() const
{
	return surface.get();
}
