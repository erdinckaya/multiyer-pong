#pragma once
#ifdef CLIENT
#include "IMessageConsumer.h"



class ClientMessageConsumer : public IMessageConsumer
{
public:
	/// Consumes server messages.
	void Consume(entityx::EntityManager& entityManager, entityx::EventManager& events, const NetworkMessageEvent& networkMessage) override;
private:
	/// Consumes client connected message and dispatches AddPlayerEvent event. That message comes only for player.
	void ClientConnected(entityx::EntityManager& entities, entityx::EventManager& events, yojimbo::Message* const message) const;
	/// Consumes start game message. Starts game then creates rival.
	void StartGame(entityx::EntityManager& entities, entityx::EventManager& events, yojimbo::Message* const message) const;
	/// Consumes UpdateGameState messages, updates game state.
	void UpdateGameState(entityx::EntityManager& entities, entityx::EventManager& events, yojimbo::Message* const message);
};


#endif