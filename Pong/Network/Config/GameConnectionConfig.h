#pragma once

#include <yojimbo.h>
#include "../../Util/Constants.h"
#include "GameChannel.h"


// the client and server config
struct GameConnectionConfig : public yojimbo::ClientServerConfig
{
	GameConnectionConfig()
	{
		protocolId = PROTOCOL_ID;
		numChannels = 2;
		channel[static_cast<int>(GameChannel::RELIABLE)].type = yojimbo::CHANNEL_TYPE_RELIABLE_ORDERED;
		channel[static_cast<int>(GameChannel::UNRELIABLE)].type = yojimbo::CHANNEL_TYPE_UNRELIABLE_UNORDERED;
	}
};
