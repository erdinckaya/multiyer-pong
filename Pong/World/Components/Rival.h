#pragma once
#include "../../Editor/IReflect.h"

/// Just a tag component.
struct Rival ADD_REFLECTION
{
	explicit Rival(const int id = 0) : id(id)
	{
	}

	int id;

	START_REFLECTION(Rival)
		ADD_REFLECTION_FIELD(id, "int")
		END_REFLECTION
};
