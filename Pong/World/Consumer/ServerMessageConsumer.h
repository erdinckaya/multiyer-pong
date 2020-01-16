#pragma once
#ifdef SERVER

#include <yojimbo.h>
#include "IMessageConsumer.h"

class ServerMessageConsumer : public IMessageConsumer
{
public:
	void Consume(entityx::EntityManager& entityManager, entityx::EventManager& events, const NetworkMessageEvent& networkMessage) override;

private:
	void UpdatePlayerMove(entityx::EntityManager& entities, yojimbo::Message* const message);
};

#endif
