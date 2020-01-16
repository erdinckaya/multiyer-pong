#pragma once

#include <linalg.h>

struct UpdatePlayerMoveEvent
{
	UpdatePlayerMoveEvent(const uint64_t& clientId = 0,
	                      const linalg::aliases::float2& pos = linalg::aliases::float2(0, 0),
	                      const linalg::aliases::float2& direction = linalg::aliases::float2(0, 0),
	                      const float& speed = 0,
	                      const uint64_t& state = 0) :
		clientId(clientId), pos(pos), direction(direction), speed(speed), state(state)
	{
	}
	
	uint64_t clientId;
	linalg::aliases::float2 pos;
	linalg::aliases::float2 direction;
	float speed;
	uint64_t state;
};
