#pragma once

#include <yojimbo.h>
#include "../../Util/Constants.h"

/// Start Game Message notifies the client or server to start the game.
class StartGameMessage : public yojimbo::Message
{
public:
	int owner;
	int botIndex;
	uint64_t players[GAME_PLAYER_MAX];

	StartGameMessage() : owner(-1), botIndex(-1)
	{
		memset(players, GAME_PLAYER_MAX, 0);
	}

	template <typename Stream>
	bool Serialize(Stream& stream)
	{
		yojimbo_serialize_int(stream, owner, -1, 20);
		yojimbo_serialize_int(stream, botIndex, -1, 20);
		for (int i = 0; i < GAME_PLAYER_MAX; ++i)
		{
			yojimbo_serialize_uint64(stream, players[i]);
		}
		
		return true;
	}

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()
};
