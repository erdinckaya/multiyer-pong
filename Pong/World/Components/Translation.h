#pragma once
#include <linalg.h>
#include "../../Editor/IReflect.h"

/// Keeps position in 2D
struct Translation ADD_REFLECTION
{
	explicit Translation(const linalg::aliases::float2& value = {0, 0}) : value(value)
	{
	}

	linalg::aliases::float2 value;

	START_REFLECTION(Translation)
		ADD_REFLECTION_FIELD(value, "float2")
		END_REFLECTION
};
