#pragma once
#include <entityx/Entity.h>


struct NetworkMessageEvent;

// Base consumer interface for multi configuration.
class IMessageConsumer
{
public:
	virtual void Consume(entityx::EntityManager& entityManager, entityx::EventManager& events, const NetworkMessageEvent& networkMessage) = 0;
};
