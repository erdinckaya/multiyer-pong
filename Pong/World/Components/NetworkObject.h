#pragma once

#include "../../Editor/IReflect.h"
#include "../../Util/Types.h"

/// Tags entity as a network object.
struct NetworkObject ADD_REFLECTION
{
	explicit NetworkObject(const NetworkObjectType& type) : value(type)
	{
	}

	NetworkObjectType value;

	START_REFLECTION(NetworkOwner)
		ADD_REFLECTION_FIELD(value, "network_object_type")
		END_REFLECTION
};
