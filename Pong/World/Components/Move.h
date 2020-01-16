#pragma once
#include <linalg.h>

#include "../../Editor/IReflect.h"

/// Keeps movement info such as speed direction and state.
struct Move ADD_REFLECTION
{
	explicit Move(const float& speed, const linalg::aliases::float2& direction = {0, 0}) : state(0), speed(speed),
	                                                                                       direction(direction)
	{
	}

	uint64_t state;
	float speed;
	linalg::aliases::float2 direction;

	START_REFLECTION(Move)
		ADD_REFLECTION_FIELD(speed, "float")
		ADD_REFLECTION_FIELD(state, "uint64_t")
		ADD_REFLECTION_FIELD(direction, "float2")
		END_REFLECTION
};
