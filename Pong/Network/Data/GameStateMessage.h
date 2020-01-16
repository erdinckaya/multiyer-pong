#pragma once

#include <yojimbo.h>
#include <linalg.h>
#include "../../Util/Constants.h"

/// Game state message that holds necessary data about game.
class GameStateMessage : public yojimbo::Message
{
public:
	uint64_t stateId;

	linalg::aliases::float2 playerPos[GAME_PLAYER_MAX];
	linalg::aliases::float2 playerMoveDirection[GAME_PLAYER_MAX];
	float playerMoveSpeed[GAME_PLAYER_MAX];
	uint64_t playerMoveState[GAME_PLAYER_MAX];
	
	linalg::aliases::float2 ballPos;
	linalg::aliases::float2 ballDirection;
	float ballSpeed;
	uint64_t ballState;

	explicit GameStateMessage() : stateId(0)
	{
	}

	template <typename Stream>
	bool Serialize(Stream& stream)
	{
		yojimbo_serialize_uint64(stream, stateId);
		yojimbo_serialize_float(stream, ballPos.x);
		yojimbo_serialize_float(stream, ballPos.y);
		yojimbo_serialize_float(stream, ballDirection.x);
		yojimbo_serialize_float(stream, ballDirection.y);
		yojimbo_serialize_float(stream, ballSpeed);
		yojimbo_serialize_uint64(stream, ballState);

		
		for (int i = 0; i < GAME_PLAYER_MAX; ++i)
		{
			yojimbo_serialize_float(stream, playerPos[i].x);
			yojimbo_serialize_float(stream, playerPos[i].y);
			yojimbo_serialize_float(stream, playerMoveDirection[i].x);
			yojimbo_serialize_float(stream, playerMoveDirection[i].y);
			yojimbo_serialize_float(stream, playerMoveSpeed[i]);
			yojimbo_serialize_uint64(stream, playerMoveState[i]);
		}
		return true;
	}

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()
};
