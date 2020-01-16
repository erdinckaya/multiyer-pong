
#ifdef SERVER

#include "../World/EntityFactory.h"
#include "../World/Events/SendGameStateEvent.h"
#include "../World/Events/AddPlayerEvent.h"
#include "../World/Events/StartGameEvent.h"
#include "../World/Events/NetworkMessageEvent.h"

#include "../Util/UtilMacro.h"

#include "GameServer.h"





GameServer::GameServer(const yojimbo::Address& address) :
	time(0.0f),
	running(true),
	adapter(this),
	server(yojimbo::GetDefaultAllocator(), DEFAULT_PRIVATE_KEY, address, connectionConfig, adapter, time)
{
	// start the server
	server.Start(GAME_PLAYER_MAX);
	CHECK_ERROR_EXIT(!server.IsRunning(), "Could not start server at port " + std::to_string(address.GetPort()));

	// print the port we got in case we used port 0
	char addressString[256];
	server.GetAddress().ToString(addressString, sizeof(addressString));
	spdlog::info("[Yojimbo] server address is {0}", addressString);

	// ... load game ...
}

/// Client connected event listener.
void GameServer::ClientConnected(int clientIndex)
{
	spdlog::info("[Yojimbo] Client {0} has connected id: {1}", clientIndex, server.GetClientId(clientIndex));
	auto canStart = true;
	for (int i = 0; i < GAME_PLAYER_MAX; i++)
	{
		canStart &= server.IsClientConnected(i);
	}

	// Create message to notify players.
	const auto message = dynamic_cast<ClientConnectedMessage*>(CreateMessage(clientIndex, GameMessageType::CLIENT_CONNECTED));
	message->clientIndex = clientIndex;
	message->clientId = server.GetClientId(clientIndex);

	// Send message and notify server engine as well.
	World::Instance.events.emit<AddPlayerEvent>(entityx::Entity(), message->clientIndex == 0, message->clientIndex, message->clientId);
	SendMessage(GameChannel::RELIABLE, clientIndex, message);

	// If all players are connected.
	if (canStart)
	{
		// Tell players that they can start game.
		for (int i = 0; i < GAME_PLAYER_MAX; i++)
		{
			const auto startMessage = dynamic_cast<StartGameMessage*>(CreateMessage(i, GameMessageType::START_GAME));
			// If bot is activated create bot.
#ifdef ACTIVATE_BOT
			startMessage->botIndex = 1;
#endif
			startMessage->owner = 0;
			startMessage->players[i] = server.GetClientId(i);
			SendMessage(GameChannel::RELIABLE, i, startMessage);
		}
		// In order to start game notify ServerGameManager.
		World::Instance.events.emit<StartGameEvent>();
	}
}

/// Client disconnected is event listener.
void GameServer::ClientDisconnected(int clientIndex)
{
	spdlog::info("[Yojimbo] Client {0} has disconnected id: {1}", clientIndex, server.GetClientId(clientIndex));
}

/// Creates messages with using yojimbo message factory.
yojimbo::Message* GameServer::CreateMessage(const int& clientIndex, const GameMessageType& type)
{
	yojimbo::Message* message = server.CreateMessage(clientIndex, static_cast<int>(type));
	return message;
}

/// Releases messages from message factory. You must release messages that created by
/// GameServer::CreateMessage, you must not release received messages unless you don't close the engine.
void GameServer::ReleaseMessage(const int& clientIndex, yojimbo::Message* message)
{
	releaseMessages.emplace_back(clientIndex, message);
}

/// Sends messages to client.
bool GameServer::SendMessage(const GameChannel& channel, const int& clientIndex, yojimbo::Message* message)
{
	if (server.IsClientConnected(clientIndex) && server.CanSendMessage(clientIndex, static_cast<int>(channel)))
	{
		server.SendMessage(clientIndex, static_cast<int>(channel), message);
		return true;
	}
	return false;
}

/// Check client connected or not.
bool GameServer::IsClientConnected(const int& clientIndex)
{
	return server.IsClientConnected(clientIndex);
}

GameServer::~GameServer() = default;

/// Main network look that runs everything.
/// This is the most important part of networking.
void GameServer::Run()
{
	while (running)
	{
		// Check time to run this loop every fixed delta time.
		const double currentTime = yojimbo_time();
		if (time <= currentTime)
		{
			Update(FIXED_SERVER_DELTA);
		}
		else
		{
			yojimbo_sleep(time - currentTime);
		}
	}
	// After everything done release all messages
	ReleaseAllMessages();
	// Release all unused messages.
	ReleaseUnusedMessages();
	// Disconnect from clients.
	server.DisconnectAllClients();
	// Stop server to remove server.
	server.Stop();
}

/// Stops network loop.
void GameServer::Stop()
{
	running = false;
}

/// Get current time
float GameServer::GetTime() const
{
	return time;
}

// DEBUG PART

/// Sets latency for debug.
void GameServer::SetLatency(float milliseconds)
{
	networkDebug.latency = milliseconds;
	server.SetLatency(milliseconds);
}

/// Sets jitter for debug
void GameServer::SetJitter(float milliseconds)
{
	networkDebug.jitter = milliseconds;
	server.SetJitter(milliseconds);
}

/// Sets package loss for debug
void GameServer::SetPacketLoss(float percent)
{
	server.SetPacketLoss(percent);
}

/// Sets duplicates for debug
void GameServer::SetDuplicates(float percent)
{
	networkDebug.duplicates = percent;
	server.SetDuplicates(percent);
}

/// Gets current network info for debug
void GameServer::GetNetworkInfo(int clientIndex, yojimbo::NetworkInfo& networkInfo) const
{
	server.GetNetworkInfo(clientIndex, networkInfo);
}

/// Release all messages that created from message factory.
void GameServer::ReleaseAllMessages()
{
	while (!releaseMessages.empty())
	{
		const auto pair = releaseMessages.front();
		server.ReleaseMessage(pair.first, pair.second);
		releaseMessages.pop_front();
	}
}

/// Releases all received messages.
void GameServer::ReleaseUnusedMessages()
{
	yojimbo::Message* message = nullptr;
	int clientIndex = -1;
	while (!receivedMessages.empty())
	{
		message = nullptr;
		clientIndex = -1;
		receivedMessages.fetch(message, clientIndex);
		if (message)
		{
			receivedMessages.remove(message);
			server.ReleaseMessage(clientIndex, message);
		}
	}
}

/// Main update loop.
void GameServer::Update(float dt)
{
	// stop if server is not running
	if (!server.IsRunning())
	{
		running = false;
		return;
	}

	// This order is very important.
	// You need to follow this order otherwise
	// You can not establish connection between server and client
	// Order is advance time, receive packet, process messages, send new packages.

	// Increase network time.
	server.AdvanceTime(time);
	time += FIXED_SERVER_DELTA;

	// Receive packages.
	server.ReceivePackets();

	// Process received messages
	ProcessMessages();

	// Send game state to all clients.
	SendGameState();

	// Send new packages.
	server.SendPackets();

	// Release all marked messages.
	ReleaseAllMessages();

}

/// Process all messages that received from users.
void GameServer::ProcessMessages()
{
	for (int i = 0; i < GAME_PLAYER_MAX; i++)
	{
		if (server.IsClientConnected(i))
		{
			for (int j = 0; j < connectionConfig.numChannels; j++)
			{
				yojimbo::Message* message = server.ReceiveMessage(i, j);
				while (message != nullptr)
				{
					ProcessMessage(i, j, message);
					message = server.ReceiveMessage(i, j);
				}
			}
		}
	}
}

/// Notify server manager to process them.
void GameServer::ProcessMessage(int clientIndex, int channelIndex, yojimbo::Message* message)
{
	receivedMessages.insert(message, clientIndex);
	World::Instance.events.emit<NetworkMessageEvent>(clientIndex, message, static_cast<GameChannel>(channelIndex));
}

/// Request game state from server game manager and send state to all players.
void GameServer::SendGameState() const
{
	auto canSend = true;
	for (int i = 0; i < GAME_PLAYER_MAX; i++)
	{
		canSend &= server.IsClientConnected(i);
	}

	// Check all player are connected or not.
	if (canSend)
	{
		World::Instance.events.emit<SendGameStateEvent>();
	}
}

#endif
