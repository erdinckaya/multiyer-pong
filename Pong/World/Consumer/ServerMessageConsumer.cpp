#ifdef SERVER

#include "ServerMessageConsumer.h"


#include "../Events/PauseEvent.h"
#include "../Events/RestartEvent.h"
#include "../../Network/Data/PlayerMoveMessage.h"


#include "../Components/PlayerNetworkInfo.h"
#include "../Events/UpdatePlayerMoveEvent.h"
#include "../Components/Move.h"

#include <spdlog/spdlog.h>
#include "../../Network/Config/GameMessageType.h"
#include "../Events/NetworkMessageEvent.h"
#include "../../Network/NetworkManager.h"


/// Consumes messages that received by server.
void ServerMessageConsumer::Consume(entityx::EntityManager& entityManager, entityx::EventManager& events,
                                    const NetworkMessageEvent& networkMessage)
{
	const auto message = networkMessage.message;
	const auto clientIndex = networkMessage.clientIndex;
	
	const auto type = static_cast<GameMessageType>(message->GetType());
	switch (type)
	{
		// Consume player move.
	case GameMessageType::PLAYER_MOVE:
		UpdatePlayerMove(entityManager, message);
		break;
		// Consume pause message
	case GameMessageType::PAUSE:
		events.emit<PauseEvent>(message);
		break;
		// Consume restart message
	case GameMessageType::RESTART:
		events.emit<RestartEvent>();
		break;
	default: break;
	}

	NetworkManager::Instance.network->ReleaseMessage(clientIndex, message);
}


void ServerMessageConsumer::UpdatePlayerMove(entityx::EntityManager& entities, yojimbo::Message* const message)
{
	// Update player's move direction.
	const auto playerMoveMessage = dynamic_cast<PlayerMoveMessage*>(message);
	entities.each<PlayerNetworkInfo, Move>(
		[playerMoveMessage](entityx::Entity entity, PlayerNetworkInfo& networkInfo, Move& move)
		{
			if (networkInfo.clientId == playerMoveMessage->clientId)
			{
				move.direction = playerMoveMessage->direction;
			}
		});
}


#endif
