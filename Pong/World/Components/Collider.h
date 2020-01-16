#pragma once

#include "../../Editor/IReflect.h"
#include "../../Util/Types.h"
#include <linalg.h>


using namespace linalg::aliases;

/// Keeps collider information of entity.
struct Collider ADD_REFLECTION
{
	explicit Collider(const ColliderType& type = ColliderType::Rectangle, const float2 front = float2(0, 0)) : type(type), front(front)
	{
	}

	ColliderType type;

	float2 front;

	START_REFLECTION(Collider)
		ADD_REFLECTION_FIELD(type, "collider_type")
		END_REFLECTION
};
