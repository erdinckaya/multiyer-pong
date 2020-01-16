#pragma once
#include <linalg.h>

#include "../../Editor/IReflect.h"

/// Keeps size in 2D
struct Size ADD_REFLECTION
{
	explicit Size(const linalg::aliases::float2& value = {1, 1}) : value(value)
	{
	}

	linalg::aliases::float2 value;

	START_REFLECTION(Size)
		ADD_REFLECTION_FIELD(value, "float2")
		END_REFLECTION
};
