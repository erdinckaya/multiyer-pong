#pragma once

#include <yojimbo.h>
#include <linalg.h>

/// Player move messages holds current direction of player.
class PlayerMoveMessage : public yojimbo::Message
{
public:
	uint64_t clientId;
	linalg::aliases::float2 direction;


	template <typename Stream>
	bool Serialize(Stream& stream)
	{
		yojimbo_serialize_uint64(stream, clientId);
		yojimbo_serialize_float(stream, direction.x);
		yojimbo_serialize_float(stream, direction.y);
		return true;
	}


	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()
};
