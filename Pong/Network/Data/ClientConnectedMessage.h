#pragma once

#include <yojimbo.h>

/// Client connection message hold client index and id.
class ClientConnectedMessage : public yojimbo::Message
{
public:
	int clientIndex;
	uint64_t clientId;

	ClientConnectedMessage() : clientIndex(-1), clientId(0)
	{
	}

	template <typename Stream>
	bool Serialize(Stream& stream)
	{
		yojimbo_serialize_int(stream, clientIndex, -1, 20);
		yojimbo_serialize_uint64(stream, clientId);
		return true;
	}

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()
};
