#pragma once
#include "../../Editor/IReflect.h"

/// Just a tag for bot.
struct Ball ADD_REFLECTION
{
	explicit Ball(const int id = 0) : id(id)
	{
	}

	int id;

	START_REFLECTION(Ball)
		ADD_REFLECTION_FIELD(id, "int")
		END_REFLECTION
};
