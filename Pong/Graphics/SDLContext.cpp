#include "SDLContext.h"
#include "../Util/UtilMacro.h"

SDLContext::SDLContext(SDLWindow& sdlWindow)
	: context(SDL_GL_CreateContext(sdlWindow), static_cast<void (&&)(SDL_GLContext)>(SDL_GL_DeleteContext))
{
	const char* error = SDL_GetError();

	CHECK_ERROR_EXIT(*error != '\0', std::string("Error initializing OpenGL context: ") + error);
}

SDLContext::operator SDL_GLContext() const
{
	return context.get();
}