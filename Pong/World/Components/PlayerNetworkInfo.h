#pragma once

#include "../../Editor/IReflect.h"

/// Keeps player's network info
struct PlayerNetworkInfo ADD_REFLECTION
{
	explicit PlayerNetworkInfo(const int& clientIndex = -1, const bool& gameOwner = false) : clientIndex(clientIndex), gameOwner(gameOwner), clientId(0)
	{
	}

	int clientIndex;
	uint64_t clientId;
	bool gameOwner;

	START_REFLECTION(PlayerNetworkInfo)
		ADD_REFLECTION_FIELD(clientIndex, "int")
		ADD_REFLECTION_FIELD(clientId, "uint64_t")
		ADD_REFLECTION_FIELD(gameOwner, "bool")
		END_REFLECTION
};
