#pragma once

#include <linalg.h>


struct UpdateBallMoveEvent
{
	UpdateBallMoveEvent(const linalg::aliases::float2& pos = linalg::aliases::float2(0, 0),
	                    const linalg::aliases::float2& direction = linalg::aliases::float2(0, 0),
	                    const float& speed = 0,
	                    const uint64_t& state = 0) :
		pos(pos), direction(direction), speed(speed), state(state)
	{
	}

	linalg::aliases::float2 pos;
	linalg::aliases::float2 direction;
	float speed;
	uint64_t state;
};
