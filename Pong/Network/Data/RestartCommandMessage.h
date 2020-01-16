#pragma once

#include <yojimbo.h>

/// Restart command message  that restarts game.
class RestartCommandMessage : public yojimbo::Message
{
public:
	uint64_t clientId;

	RestartCommandMessage() : clientId(0)
	{
	}

	template <typename Stream>
	bool Serialize(Stream& stream)
	{
		yojimbo_serialize_uint64(stream, clientId);

		return true;
	}

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()
};
