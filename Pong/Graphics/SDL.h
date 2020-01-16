#pragma once
#include <SDL2/SDL.h>

/// Initializes SDL2 with given flags.
class SDL
{
public:
	SDL(int flags);

	~SDL();

private:
	SDL(const SDL&); //Unimplemented
};
