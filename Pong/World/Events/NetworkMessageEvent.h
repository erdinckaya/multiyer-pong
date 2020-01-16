#pragma once

#include <yojimbo.h>
#include "../../Network/Config/GameChannel.h"


struct NetworkMessageEvent
{
#ifdef CLIENT
	explicit NetworkMessageEvent(yojimbo::Message* message = nullptr, GameChannel channel = GameChannel::RELIABLE) : message(message), channel(channel)
	{
	}
#else
	explicit NetworkMessageEvent(const int& clientIndex = -1, yojimbo::Message* message = nullptr, const GameChannel channel = GameChannel::RELIABLE) : clientIndex(clientIndex), message(message), channel(channel)
	{
	}

	int clientIndex;
#endif
	yojimbo::Message* message;
	GameChannel channel;
};
