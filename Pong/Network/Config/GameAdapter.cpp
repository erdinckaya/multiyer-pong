#include "GameAdapter.h"
#include "../GameServer.h"

void GameAdapter::OnServerClientConnected(int clientIndex)
{
#ifdef SERVER
	if (server != nullptr)
	{
		server->ClientConnected(clientIndex);
	}
#endif
}

void GameAdapter::OnServerClientDisconnected(int clientIndex)
{
#ifdef SERVER
	if (server != nullptr)
	{
		server->ClientDisconnected(clientIndex);
	}
#endif
}
