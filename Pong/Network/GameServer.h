#pragma once

#ifdef SERVER
#include "../Util/Map.h"


#include <yojimbo.h>

#include "Config/GameConnectionConfig.h"
#include "Config/GameAdapter.h"
#include "../Util/Deque.h"
#include "Base/INetwork.h"

#ifdef SendMessage
#undef SendMessage
#endif



struct NetworkMessageEvent;

/// Game server only runs its own thread so that no need to syncronize it.
///It sends game state server thread.
class GameServer : public INetwork
{
public:

	explicit GameServer(const yojimbo::Address& address);

	~GameServer() override;

	void ClientConnected(int clientIndex);

	void ClientDisconnected(int clientIndex);

	yojimbo::Message* CreateMessage(const int& clientIndex, const GameMessageType& type) override;

	void ReleaseMessage(const int& clientIndex, yojimbo::Message* message) override;

	bool SendMessage(const GameChannel& channel, const int& clientIndex, yojimbo::Message* message) override;
	
	bool IsClientConnected(const int& clientIndex) override;

	void Run() override;
	
	void Stop() override;

	float GetTime() const override;

	void SetLatency(float milliseconds) override;
	void SetJitter(float milliseconds) override;
	void SetPacketLoss(float percent) override;
	void SetDuplicates(float percent) override;
	void GetNetworkInfo(int clientIndex, yojimbo::NetworkInfo& networkInfo) const override;
private:

	
	float time;
	std::atomic<bool> running;

	GameAdapter adapter;
	GameConnectionConfig connectionConfig;
	yojimbo::Server server;
	
	Deque<std::pair<int, yojimbo::Message*>> releaseMessages;
	Map<yojimbo::Message*, int> receivedMessages;

	void Update(float dt);

	void ProcessMessages();

	void ProcessMessage(int clientIndex, int channelIndex, yojimbo::Message* message);

	void SendGameState() const;
	
	void ReleaseAllMessages();
	
	void ReleaseUnusedMessages();

	
};
#endif
