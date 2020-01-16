#pragma once
#include "../../Editor/IReflect.h"

/// Just a tag component for player.
struct Player ADD_REFLECTION
{
	explicit Player(const int id = 0) : id(id)
	{
	}

	int id;

	START_REFLECTION(Player)
		ADD_REFLECTION_FIELD(id, "int")
		END_REFLECTION
};
