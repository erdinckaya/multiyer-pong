#pragma once
#include <linalg.h>
#include "../../Util/Deque.h"

/// Keeps player's position history.
struct PlayerMoveHistory
{
	Deque < std::pair < uint64_t, float2>> queue;
};
