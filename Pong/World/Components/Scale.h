#pragma once
#include <linalg.h>
#include "../../Editor/IReflect.h"

/// Keeps scale in 2D
struct Scale ADD_REFLECTION
{
	explicit Scale(const linalg::aliases::float2& value = {1, 1}) : value(value)
	{
	}

	linalg::aliases::float2 value;

	START_REFLECTION(Scale)
		ADD_REFLECTION_FIELD(value, "float2")
		END_REFLECTION
};
