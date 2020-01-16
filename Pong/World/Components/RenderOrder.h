#pragma once
#include "../../Editor/IReflect.h"

/// Keeps render order of shape.
struct RenderOrder ADD_REFLECTION
{
	explicit RenderOrder(const int& value = 0) : value(value)
	{
	}

	int value;

	START_REFLECTION(RenderOrder)
		ADD_REFLECTION_FIELD(value, "int")
		END_REFLECTION
};