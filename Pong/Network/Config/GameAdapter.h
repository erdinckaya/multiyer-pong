#pragma once

#include <yojimbo.h>
#include "GameMessageFactory.h"

class GameServer;
class GameClient;

/// Adapter class that subscribes server or client's features.
class GameAdapter : public yojimbo::Adapter
{
public:
#ifdef SERVER
	explicit GameAdapter(GameServer* server = nullptr)
	{
		this->server = server;
	}
#else
	explicit GameAdapter(GameClient* client = nullptr)
	{
		this->client = client;
	}
#endif

	yojimbo::MessageFactory* CreateMessageFactory(yojimbo::Allocator& allocator) override
	{
		return YOJIMBO_NEW(allocator, GameMessageFactory, allocator);
	}

	void OnServerClientConnected(int clientIndex) override;

	void OnServerClientDisconnected(int clientIndex) override;

private:
#ifdef SERVER
	GameServer* server;
#else
	GameClient* client;
#endif
	
	
};
