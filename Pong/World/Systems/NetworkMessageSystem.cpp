#include "NetworkMessageSystem.h"


#ifdef SERVER
#include "../Consumer/ServerMessageConsumer.h"
#else
#include "../Consumer/ClientMessageConsumer.h"
#endif




NetworkMessageSystem::NetworkMessageSystem()
{
#ifdef SERVER
	consumer = std::make_unique<ServerMessageConsumer>();
#else
	consumer = std::make_unique<ClientMessageConsumer>();
#endif
}

void NetworkMessageSystem::configure(entityx::EventManager& events)
{
	events.subscribe<NetworkMessageEvent>(*this);
}

void NetworkMessageSystem::update(ex::EntityManager& entities, ex::EventManager& events, ex::TimeDelta dt)
{
	while (!messages.empty())
	{
		const auto networkMessage = messages.front();
		consumer->Consume(entities, events, networkMessage);
		messages.pop_front();
	}
}


void NetworkMessageSystem::receive(const NetworkMessageEvent& networkEvent)
{
	messages.push_back(networkEvent);
}
