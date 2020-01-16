#pragma once

#ifdef CLIENT
#include "../Util/Set.h"


#include <yojimbo.h>

#include "Config/GameConnectionConfig.h"
#include "Config/GameAdapter.h"
#include "Base/INetwork.h"
#include "../Util/Deque.h"

/// Remove Windows SendMessage macro.
#ifdef SendMessage
#undef SendMessage
#endif


struct NetworkMessageEvent;

/// Game client only runs its own thread so that no need to syncronize it.
/// It creates connection between server and player.
class GameClient : public INetwork
{
public:
	explicit GameClient();

	~GameClient();

	int Connect(uint64_t clientId, uint8_t* connectToken);

	int InsecureConnect();

	uint64_t GetClientId() const override;

	void GetNetworkInfo(yojimbo::NetworkInfo& networkInfo) const override;

	void Run() override;
	
	void Stop() override;

	yojimbo::Message* CreateMessage(const GameMessageType& type) override;

	void ReleaseMessage(yojimbo::Message* message) override;

	bool SendMessage(const GameChannel& channel, yojimbo::Message* message) override;

	float GetTime() const override;

	void SetLatency(float milliseconds) override;
	void SetJitter(float milliseconds) override;
	void SetPacketLoss(float percent) override;
	void SetDuplicates(float percent) override;
private:
	float time;
	std::atomic<bool> running;

	GameAdapter adapter;
	GameConnectionConfig config;
	yojimbo::Client client;
	Deque<yojimbo::Message*> releaseMessages;
	Set<yojimbo::Message*> receivedMessages;
	std::mutex locker;


	void Update(float dt);

	void ProcessMessages();
	
	void ReleaseAllMessages();
	void ReleaseUnusedMessages();

	void ProcessMessage(const GameChannel& channel, yojimbo::Message* message);
};
#endif
