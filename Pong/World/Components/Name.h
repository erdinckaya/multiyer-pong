#pragma once

#include "../../Editor/IReflect.h"

/// Keeps name of entity for the sake of debug.
struct Name ADD_REFLECTION
{
	explicit Name(const std::string& value) : value(value)
	{
	}

	std::string value;

	START_REFLECTION(Name)
		ADD_REFLECTION_FIELD(value, "string")
		END_REFLECTION
};
