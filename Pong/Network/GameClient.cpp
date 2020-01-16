
#ifdef CLIENT
#include "GameClient.h"
#include "../World/World.h"
#include "../World/Events/NetworkMessageEvent.h"

GameClient::GameClient()
	: time(0.0f),
	  running(false),
	  adapter(this), client(yojimbo::GetDefaultAllocator(), yojimbo::Address("0.0.0.0"), config, adapter, time)
{
	
}

GameClient::~GameClient() = default;

/// Creates connection with created token.
int GameClient::Connect(uint64_t clientId, uint8_t* connectToken)
{
	client.Connect(clientId, connectToken);
	if (client.IsDisconnected())
	{
		return -1;
	}

	char addressString[256];
	client.GetAddress().ToString(addressString, sizeof(addressString));
	spdlog::info("[Yojimbo] client address is {0}", addressString);

	return 1;
}

/// Creates insecure connection between player and server, bt creating its client id and with default private key.
int GameClient::InsecureConnect()
{
	// Create client id.
	uint64_t clientId = 0;
	yojimbo::random_bytes(reinterpret_cast<uint8_t*>(&clientId), 8);
	spdlog::info("[Yojimbo] client id is {}", clientId);

	// Connect to server.
	client.InsecureConnect(DEFAULT_PRIVATE_KEY, clientId, yojimbo::Address(SERVER_ADDRESS, SERVER_PORT));
	if (client.IsDisconnected())
	{
		spdlog::critical("Client couldn't connect");
		return -1;
	}

	char addressString[256];
	client.GetAddress().ToString(addressString, sizeof(addressString));
	spdlog::info("[Yojimbo] client address is {0}", addressString);

	return 1;
}

// Get client id.
uint64_t GameClient::GetClientId() const
{
	return client.GetClientId();
}

// Release all messages.
void GameClient::ReleaseAllMessages()
{
	yojimbo::Message* message = nullptr;
	while (!releaseMessages.empty())
	{
		message = releaseMessages.front();
		receivedMessages.remove(message);
		client.ReleaseMessage(message);
		releaseMessages.pop_front();
	}

}

/// Releases all received messages.
void GameClient::ReleaseUnusedMessages()
{
	yojimbo::Message* message = nullptr;
	while (!receivedMessages.empty())
	{
		message = nullptr;
		receivedMessages.fetch(message);
		if (message)
		{
			receivedMessages.remove(message);
			client.ReleaseMessage(message);

		}
	}
}

/// Main network look that runs everything.
/// This is the most important part of networking.
void GameClient::Run()
{
	InsecureConnect();
	running = true;
	while (running)
	{
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
	spdlog::info("[Yojimbo] Disconnecting!!");

	// Release markeds messages.
	ReleaseAllMessages();

	// Release received messages.
	ReleaseUnusedMessages();
	// Disconnect client.
	client.Disconnect();
	
}

/// Stops clients 
void GameClient::Stop()
{
	running = false;
}

/// Creates messages with using yojimbo message factory.
yojimbo::Message* GameClient::CreateMessage(const GameMessageType& type)
{
	const std::lock_guard<std::mutex> guard(locker);
	yojimbo::Message* message = client.CreateMessage(static_cast<int>(type));
	return message;
}

/// Releases messages from message factory. You must release messages that created by
/// GameClient::CreateMessage, you must not release received messages unless you don't close the engine.
void GameClient::ReleaseMessage(yojimbo::Message* message)
{
	if (message)
	{
		releaseMessages.push_back(message);
	}
}

/// Sends messages to server.
bool GameClient::SendMessage(const GameChannel& channel, yojimbo::Message* message)
{
	const std::lock_guard<std::mutex> guard(locker);
	if (client.IsConnected() && client.CanSendMessage(static_cast<int>(channel)))
	{
		client.SendMessage(static_cast<int>(channel), message);
	}

	return true;
}

/// Gets current time.
float GameClient::GetTime() const
{
	return time;
}


// DEBUG PART


/// Sets latency for debug.
void GameClient::SetLatency(float milliseconds)
{
	networkDebug.latency = milliseconds;
	client.SetLatency(milliseconds);
}

/// Sets jitter for debug
void GameClient::SetJitter(float milliseconds)
{
	networkDebug.jitter = milliseconds;
	client.SetJitter(milliseconds);
}

/// Sets package loss for debug
void GameClient::SetPacketLoss(float percent)
{
	client.SetPacketLoss(percent);
}

/// Sets duplicates for debug
void GameClient::SetDuplicates(float percent)
{
	networkDebug.duplicates = percent;
	client.SetDuplicates(percent);
}

/// Main update loop.
void GameClient::Update(float dt)
{
	const std::lock_guard<std::mutex> guard(locker);

	if (client.IsDisconnected())
	{
		running = false;
		return;
	}

	// This order is very important.
	// You need to follow this order otherwise
	// You can not establish connection between server and client
	// Order is advance time, receive packet, process messages, send new packages.

	// Increase network time.
	client.AdvanceTime(time);
	time += dt;

	// Receive packages.
	client.ReceivePackets();


	if (client.ConnectionFailed())
	{
		running = false;
		spdlog::info("[Yojimbo] Connection failed!!");
		return;
	}

	// Process received messages
	ProcessMessages();

	// Send new packages.
	client.SendPackets();

	// Release all marked messages.
	ReleaseAllMessages();
}

/// Process all messages that received from server.
void GameClient::ProcessMessages()
{
	if (client.IsConnected())
	{
		for (int i = 0; i < config.numChannels; i++)
		{
			yojimbo::Message* message = client.ReceiveMessage(i);

			while (message != nullptr)
			{
				ProcessMessage(static_cast<GameChannel>(i), message);
				message = client.ReceiveMessage(i);
			}
		}
	}
}

/// Notify client manager to process them.
void GameClient::ProcessMessage(const GameChannel& channel, yojimbo::Message* message)
{
	receivedMessages.insert(message);
	World::Instance.events.emit<NetworkMessageEvent>(message, channel);
}

/// Gets current network info for debug
void GameClient::GetNetworkInfo(yojimbo::NetworkInfo& networkInfo) const
{
	client.GetNetworkInfo(networkInfo);
}

#endif
