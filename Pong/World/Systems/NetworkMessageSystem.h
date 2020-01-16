#pragma once

#include <entityx/entityx.h>
#include "../../Util/Deque.h"
#include "../Events/NetworkMessageEvent.h"
#include "../Consumer/IMessageConsumer.h"


namespace ex = entityx;

class NetworkMessageSystem : public ex::System<NetworkMessageSystem>, public ex::Receiver<NetworkMessageSystem>
{
public:
	/// Initialize consumer respect to configuration, which can be ClientMessageConsumer or ServerMessageConsumer
	NetworkMessageSystem();

	/// Configures system, and subscribes to NetworkMessageEvent;
	void configure(entityx::EventManager& events) override;

	/// Fetches network messages and passes to message consumer.
	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;

	/// NetworkMessageEvent receiver, gets messages and saves them in deque,
	/// By doing this we are changing thread server to main thread, hence we can
	/// execute our processes in single thread.
	void receive(const NetworkMessageEvent& networkEvent);
private:
	// Thread safe deque for received messages from server.
	Deque<NetworkMessageEvent> messages;
	// Message consumer. It could be server or client.
	std::unique_ptr<IMessageConsumer> consumer;
};


