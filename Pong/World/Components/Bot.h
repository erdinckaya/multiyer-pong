#pragma once

#include "../../Editor/IReflect.h"

/// Just a tag for bot
struct Bot ADD_REFLECTION
{
	explicit Bot(const bool& enabled= true) : enabled(enabled)
	{
	}

	bool enabled;

	START_REFLECTION(Bot)
		ADD_REFLECTION_FIELD(enabled, "bool")
		END_REFLECTION
};
