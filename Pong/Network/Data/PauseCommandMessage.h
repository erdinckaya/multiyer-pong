#pragma once

#include <yojimbo.h>

/// Pause command messages that pauses or resumes the game.
class PauseCommandMessage : public yojimbo::Message
{
public:
	uint64_t clientId;
	bool value;

	PauseCommandMessage() : clientId(0), value(true)
	{
		
	}

	template <typename Stream>
	bool Serialize(Stream& stream)
	{
		yojimbo_serialize_uint64(stream, clientId);
		yojimbo_serialize_bool(stream, value);
		
		return true;
	}

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()
};
