#include "PlayerMoveSystem.h"
#include "../Components/PlayerMoveHistory.h"
#include "../Components/Move.h"
#include "../Components/PlayerNetworkInfo.h"
#include "../../Network/NetworkManager.h"


#include "../Components/Translation.h"
#include "../Components/Size.h"
#include "../../Input/KeyInput.h"

#include "../../Util/Constants.h"
#include "../../Util/UtilMacro.h"
#include "../Components/Player.h"
#include "../Events/UpdatePlayerMoveEvent.h"
#include "../../Network/Data/PlayerMoveMessage.h"
#include "../../Network/Config/GameChannel.h"
#include "../../Network/Config/GameMessageType.h"


PlayerMoveSystem::PlayerMoveSystem() : entities(nullptr)
{
}


void PlayerMoveSystem::configure(entityx::EntityManager& entities, entityx::EventManager& events)
{
	this->entities = &entities;
	events.subscribe<UpdatePlayerMoveEvent>(*this);
}


void PlayerMoveSystem::update(ex::EntityManager& entities, ex::EventManager& events, ex::TimeDelta dt)
{
	/// For every paddle.
	entities.each<Move, Translation, Size, PlayerMoveHistory, PlayerNetworkInfo>(
		[&dt](ex::Entity entity, Move& move, Translation& translation, Size& size, PlayerMoveHistory& history,
		   PlayerNetworkInfo& networkInfo)
		{
#ifdef CLIENT
			// Checks paddle is player or rival. If player handle inputs.
			const auto isPlayer = entity.has_component<Player>();
			if (isPlayer)
			{
				// Handle inputs.
				HandleInputs(move);
			}
			// Get old translation to compare with new one to decide whether change move state or not.
			const auto oldTranslation = translation.value;
#endif
			// Integrate player's move.
			Integrate(translation, move, size, dt);
#ifdef CLIENT
			if (isPlayer)
			{
				// Calculate distance between new one old one to decide player is stopping or not.
				const auto distance = std::abs(translation.value.x - oldTranslation.x);
				// Always compare floating points with tolerance.
				const float tolerance = 2.0f;
				if (distance > tolerance)
				{
					// Add old state to history to check future server states.
					// In order to provide a smooth game play to player we need to move our player
					// immediately, after that we can wait our next server state to correct our decision.
					// If we reach the max queue size pop front to make place to new one.
					if (history.queue.size() > PLAYER_POSITION_HISTORY_SIZE)
					{
						history.queue.pop_front();
					}
					// Add history to queue
					history.queue.emplace_back(move.state, translation.value);
				}
				// Send our current position and direction to server to correct it.
				SendPlayerMoveMessage(move.direction, networkInfo.clientId);
			}
#endif
		});
}

#ifdef CLIENT
void PlayerMoveSystem::HandleInputs(Move& move)
{
	// Move left
	if (KeyInput::Instance.IsKeyHeld(SDL_SCANCODE_LEFT) && !KeyInput::Instance.IsKeyHeld(SDL_SCANCODE_RIGHT))
	{
		move.direction = float2(-1, 0);
	}
	// Move right
	else if (KeyInput::Instance.IsKeyHeld(SDL_SCANCODE_RIGHT) && !KeyInput::Instance.IsKeyHeld(SDL_SCANCODE_LEFT))
	{
		move.direction = float2(1, 0);
	}
	// Otherwise stop.
	else
	{
		move.direction = float2(0, 0);
	}
}


void PlayerMoveSystem::SendPlayerMoveMessage(const float2& direction, const uint64_t& clientId)
{
	// Create player move message.
	auto playerMoveMessage = dynamic_cast<PlayerMoveMessage*>(NetworkManager::Instance.network->CreateMessage(GameMessageType::PLAYER_MOVE));
	// Assign it.
	playerMoveMessage->clientId = clientId;
	playerMoveMessage->direction = direction;
	// Send it to server to correct and pass the player's movement direction.
	NetworkManager::Instance.network->SendMessage(GameChannel::UNRELIABLE, playerMoveMessage);
}
#endif


void PlayerMoveSystem::Integrate(Translation& translation, Move& move, const Size& size, const float& dt)
{
	const auto oldTranslation = translation.value;
	translation.value += move.speed * static_cast<float>(dt) * move.direction;
	const auto left = (-CLIENT_WINDOW_WIDTH + size.value.x) * 0.5f;
	const auto right = (CLIENT_WINDOW_WIDTH - size.value.x) * 0.5f;
	CLAMP(translation.value.x, left, right);
	const auto distance = std::abs(oldTranslation.x - translation.value.x);
	// If it does not stop increase move state.
	if (distance  > 2.0f)
	{
		move.state++;
	}
}

void PlayerMoveSystem::receive(const UpdatePlayerMoveEvent& event)
{
#ifdef CLIENT
	// For every paddle.
	entities->each<Move, Translation, Size, PlayerMoveHistory, PlayerNetworkInfo>(
		[&event](ex::Entity entity, Move& move, Translation& translation, Size& size, PlayerMoveHistory& history,
			PlayerNetworkInfo& networkInfo)
	{
		const auto isPlayer = entity.has_component<Player>();
		// Check this movement belongs to yours.
		if (event.clientId == networkInfo.clientId)
		{
			// Check if it is player for client. That means 
			if (isPlayer)
			{
				// Check player history whether we passed this state or not.
				auto found = false;
				while (!history.queue.empty())
				{
					const auto& pair = history.queue.front();
					const auto& state = pair.first;

					// If state is older than server, we dont need that state anymore so that we can
					// delete this state.
					if (event.state > state)
					{
						history.queue.pop_front();
						continue;
					}

					// If we passed that state do nothing.
					if (event.state == state)
					{
						found = true;
						break;
					}

					DEBUG_LOG("=======================");
					DEBUG_LOG("Event state {0} pos x: {1}, y: {2}", event.state, event.pos.x, event.pos.y);
					const auto copy = history.queue.copy();
					for (auto& p : copy)
					{
						DEBUG_LOG("History state {0} pos x: {1}, y: {2}", p.first, p.second.x, p.second.y);
					}
					DEBUG_LOG("=======================");
					break;
					
				}

				// If we couldn't find the state so correct it with servers' state and position.
				if (!found)
				{
					history.queue.clear();
					translation.value = event.pos;
					move.state = event.state;
					history.queue.emplace_back(move.state, translation.value);
				}
			}
			else // If it is not player so we dont care its movement because server handles it, assign it.
			{
				move.state = event.state;
			}
			// In any case we need to assign corrected speed and direction because it is server decides every
			// important decisions.
			move.direction = event.direction;
			move.speed = event.speed;
		}
		
	});
#endif

}
