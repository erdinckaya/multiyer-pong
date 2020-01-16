
#ifdef CLIENT

#include "../../Network/Data/StartGameMessage.h"
#include "../../Network/Data/GameStateMessage.h"
#include "../../Util/UtilMacro.h"

#include "../Components/Ball.h"
#include "../Components/Move.h"
#include "../Events/UpdateBallMoveEvent.h"
#include "../Components/Bot.h"
#include "../Events/PauseEvent.h"
#include "../Components/Translation.h"
#include "../Events/UpdatePlayerMoveEvent.h"

#include "ClientMessageConsumer.h"

#include "../../Network/Data/ClientConnectedMessage.h"
#include "../Components/PlayerNetworkInfo.h"
#include "../Components/Player.h"
#include "../Components/Rival.h"
#include "../Events/AddPlayerEvent.h"
#include "../Events/StartGameEvent.h"
#include "../Events/NetworkMessageEvent.h"
#include "../../Network/NetworkManager.h"
#include "../../Network/Config/GameMessageType.h"
#include "../Events/RestartEvent.h"



#include <sstream>

void ClientMessageConsumer::Consume(entityx::EntityManager& entityManager, entityx::EventManager& events,
                                    const NetworkMessageEvent& networkMessage)
{
	const auto message = networkMessage.message;
	const auto type = static_cast<GameMessageType>(message->GetType());
	switch (type)
	{
	case GameMessageType::CLIENT_CONNECTED:
		{
			DEBUG_LOG("Client has accepted by server ");
			ClientConnected(entityManager, events, message);
			break;
		}
	case GameMessageType::START_GAME:
		{
			DEBUG_LOG("Game has started!");
			StartGame(entityManager, events, message);
			break;
		}
	case GameMessageType::GAME_STATE:
		{
			UpdateGameState(entityManager, events, message);
			break;
		}
	case GameMessageType::PAUSE:
		{
			events.emit<PauseEvent>(message);
			break;
		}
	case GameMessageType::RESTART:
	{
		events.emit<RestartEvent>();
		break;
	}
	default: break;
	}

	std::thread::id this_id = std::this_thread::get_id();
	std::ostringstream stream;
	stream << this_id;

	//DEBUG_LOG("REMOVE MESSAGE ClientMessageConsumer id {0} type {1} thread {2}", message->GetId(), message->GetType(), stream.str());
	NetworkManager::Instance.network->ReleaseMessage(message);
}

void ClientMessageConsumer::ClientConnected(entityx::EntityManager& entities, entityx::EventManager& events,
                                            yojimbo::Message* const message) const
{
	const auto clientConnected = dynamic_cast<ClientConnectedMessage*>(message);
	spdlog::debug("ClientConnected message has received! clientId {0} clientIndex {1}", clientConnected->clientId,
	              clientConnected->clientIndex);
	entities.each<PlayerNetworkInfo, Player>(
		[clientConnected, &events](entityx::Entity entity, PlayerNetworkInfo& networkInfo, Player& player)
		{
			networkInfo.clientIndex = clientConnected->clientIndex;
			networkInfo.clientId = clientConnected->clientId;
			events.emit<AddPlayerEvent>(entity, false, networkInfo.clientIndex, networkInfo.clientId);
		});
}

void ClientMessageConsumer::StartGame(entityx::EntityManager& entities, entityx::EventManager& events,
                                      yojimbo::Message* const message) const
{
	const auto startGame = dynamic_cast<StartGameMessage*>(message);
	const auto clientId = NetworkManager::Instance.network->GetClientId();
	// Print debug.
	for (int i = 0; i < GAME_PLAYER_MAX; ++i)
	{
		spdlog::debug("Player{0} clientId {1}", i, startGame->players[i]);
	}
	entities.each<PlayerNetworkInfo>(
		[startGame, &events, &clientId](entityx::Entity entity, PlayerNetworkInfo& networkInfo)
		{
			networkInfo.gameOwner = networkInfo.clientId == startGame->players[startGame->owner];
			// Create rival.
			if (entity.has_component<Rival>())
			{
				for (int i = 0; i < GAME_PLAYER_MAX; ++i)
				{
					if (startGame->players[i] != clientId)
					{
						networkInfo.clientIndex = i;
						networkInfo.clientId = startGame->players[i];
						events.emit<AddPlayerEvent>(entity, true, networkInfo.clientIndex, networkInfo.clientId);
					}
				}
			}
#ifdef ACTIVATE_BOT
			// Activate bot if needed.
			if (startGame->botIndex == networkInfo.clientIndex)
			{
				DEBUG_CLIENT(info, "Bot is activated player 1 is now bot and it is uncontrollable. Plase control player 0!");
				entity.assign<Bot>();
			}
#endif
		});
	// Dispatch StartGameEvent
	events.emit<StartGameEvent>();
}

void ClientMessageConsumer::UpdateGameState(entityx::EntityManager& entities, entityx::EventManager& events,
                                            yojimbo::Message* const message)
{
	const auto gameState = dynamic_cast<GameStateMessage*>(message);

	// If player is not game owner we have to flip positions and directions in y axis.
	// Because server keeps positions respect to game owner whose client id is always zero.
	auto flipYAxis = false;
	entities.each<PlayerNetworkInfo, Translation>(
		[&events, gameState, &flipYAxis](entityx::Entity entity, PlayerNetworkInfo& networkInfo,
		                                 Translation& translation)
		{
			flipYAxis = !(NetworkManager::Instance.network->GetClientId() == networkInfo.clientId && networkInfo.
				gameOwner);
			// Check for flip.
			const auto axis = flipYAxis ? -1.0f : 1.0f;

			// Dispatch event for player movement.
			const auto clientId = networkInfo.clientId;
			auto pos = gameState->playerPos[networkInfo.clientIndex];
			pos.y *= axis;
			const auto direction = gameState->playerMoveDirection[networkInfo.clientIndex];
			const auto speed = gameState->playerMoveSpeed[networkInfo.clientIndex];
			const auto state = gameState->playerMoveState[networkInfo.clientIndex];

			events.emit<UpdatePlayerMoveEvent>(clientId, pos, direction, speed, state);
		});

	entities.each<Ball, Move>([gameState, flipYAxis, &events](entityx::Entity entity, Ball& ball, Move& move)
	{
		// Flip if needed and dispatch event.
		const auto axis = flipYAxis ? -1.0f : 1.0f;
		auto pos = gameState->ballPos;
		pos.y *= axis;
		auto direction = gameState->ballDirection;
		direction.y *= axis;
		const auto speed = gameState->ballSpeed;
		const auto state = gameState->ballState;
		events.emit<UpdateBallMoveEvent>(pos, direction, speed, state);
	});
}


#endif
