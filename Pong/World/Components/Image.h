#pragma once

#include "../../Editor/IReflect.h"
#include "../../Util/Types.h"

/// Keeps image asset information.
struct Image ADD_REFLECTION
{
	explicit Image(SpriteType asset = SpriteType::Count, bool realSize = true) : value(asset), realSize(realSize)
	{
	}

	SpriteType value;
	bool realSize;

	START_REFLECTION(Image)
		ADD_REFLECTION_FIELD(value, "sprite_type")
		ADD_REFLECTION_FIELD(realSize, "bool")
		END_REFLECTION
};
