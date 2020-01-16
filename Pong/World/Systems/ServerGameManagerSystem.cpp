#ifdef SERVER

#include "ServerGameManagerSystem.h"


#include "../../Network/Data/GameStateMessage.h"
#include "../../Network/Config/GameMessageType.h"
#include "../../Network/NetworkManager.h"

#include "../Events/PauseEvent.h"
#include "../Events/RestartEvent.h"
#include "../Events/ChangeTextEvent.h"
#include "../Events/AddPlayerEvent.h"

#include "../EntityFactory.h"
#include "../../Network/Data/PauseCommandMessage.h"
#include "../../Network/Config/GameChannel.h"
#include "../../Network/Config/GameMessageType.h"


#ifdef ACTIVATE_BOT
#include "../Components/Bot.h"
#endif

#ifdef EDITOR
#include "../../Editor/Editor.h"
#endif


#ifdef SendMessage
#undef SendMessage
#endif

ServerGameManagerSystem::ServerGameManagerSystem() : entities(nullptr), events(nullptr), stateId(0)
{
}

void ServerGameManagerSystem::update(EntityManager& entities, EventManager& events, TimeDelta dt)
{
}


void ServerGameManagerSystem::configure(EntityManager& entities, EventManager& events)
{
	this->entities = &entities;
	this->events = &events;

	stateId = 0;
	events.subscribe<AddPlayerEvent>(*this);
	events.subscribe<StartGameEvent>(*this);
	events.subscribe<SendGameStateEvent>(*this);
	events.subscribe<PauseEvent>(*this);
	events.subscribe<RestartEvent>(*this);
}


void ServerGameManagerSystem::receive(const AddPlayerEvent& addPlayerEvent)
{
	entityx::Entity ent;

	// Check for zero index and add it as player.
	if (addPlayerEvent.clientIndex == 0)
	{
		entities->each<PlayerNetworkInfo, Player>(
			[&addPlayerEvent, &ent](ex::Entity entity, PlayerNetworkInfo& networkInfo, Player& player)
			{
				networkInfo.clientIndex = addPlayerEvent.clientIndex;
				networkInfo.clientId = addPlayerEvent.clientId;
				networkInfo.gameOwner = addPlayerEvent.owner;
				ent = entity;
			});
	}
	else // Otherwise you can add rival, furthermore if bot is activated, add bot component as well.
	{
		entities->each<PlayerNetworkInfo, Rival>(
			[&addPlayerEvent, &ent](ex::Entity entity, PlayerNetworkInfo& networkInfo, Rival& rival)
			{
				networkInfo.clientIndex = addPlayerEvent.clientIndex;
				networkInfo.clientId = addPlayerEvent.clientId;
				networkInfo.gameOwner = addPlayerEvent.owner;
#ifdef ACTIVATE_BOT
				// Add bot component.
				DEBUG_SERVER(info, "Bot is activated player 1 is now bot!");
				entity.assign<Bot>();
#endif

				ent = entity;
			});
	}

	// After all finish players and paddles components.
	EntityFactory::AddPaddle(ent, addPlayerEvent.clientIndex == 1);
}


void ServerGameManagerSystem::receive(const StartGameEvent& startGameEvent)
{
	EntityFactory::CreateBall();
}


void ServerGameManagerSystem::receive(const SendGameStateEvent& sendGameStateEvent)
{
	// for every client.
	for (int i = 0; i < GAME_PLAYER_MAX; ++i)
	{
		// Craete game state message.
		auto message = dynamic_cast<GameStateMessage*>(NetworkManager::Instance
		                                               .network->CreateMessage(i, GameMessageType::GAME_STATE));
		// Set state id and positions.
		message->stateId = stateId;
		entities->each<PlayerNetworkInfo, Translation, Move>(
			[message](ex::Entity entity, PlayerNetworkInfo& networkInfo, Translation& translation, Move& move)
			{
				message->playerPos[networkInfo.clientIndex] = translation.value;
				message->playerMoveDirection[networkInfo.clientIndex] = move.direction;
				message->playerMoveSpeed[networkInfo.clientIndex] = move.speed;
				message->playerMoveState[networkInfo.clientIndex] = move.state;
			});
		// Set ball info.
		entities->each<Ball, Translation, Move>(
			[message](ex::Entity entity, Ball& ball, Translation& translation, Move& move)
			{
				message->ballPos = translation.value;
				message->ballSpeed = move.speed;
				message->ballState = move.state;
				message->ballDirection = move.direction;
			});

		// Send it to clients.
		NetworkManager::Instance.network->SendMessage(GameChannel::UNRELIABLE, i, message);
	}
	// Increase state.
	stateId++;
}

void ServerGameManagerSystem::receive(const PauseEvent& pauseEvent)
{
	// Toggles pause.
	World::Instance.PauseWorld(!World::Instance.IsWorldPaused());

	// Sends pause message to clients.
	for (int i = 0; i < GAME_PLAYER_MAX; ++i)
	{
		const auto pauseMessage = dynamic_cast<PauseCommandMessage*>(NetworkManager::Instance
		                                                             .network->CreateMessage(i, GameMessageType::PAUSE)
		);
		pauseMessage->value = World::Instance.IsWorldPaused();
		NetworkManager::Instance.network->SendMessage(GameChannel::RELIABLE, i, pauseMessage);
	}

	// Update pause text in the scene.
	events->emit<ChangeTextEvent>(TextId::Pause, "Pause: " + std::to_string(World::Instance.IsWorldPaused()));
}

void ServerGameManagerSystem::receive(const RestartEvent& restartEvent)
{
	Restart();
}

void ServerGameManagerSystem::Restart() const
{
	entities->each<Paddle, Translation, Move, Size, Scale, PlayerMoveHistory>(
		[](ex::Entity entity, Paddle& paddle, Translation& translation, Move& move, Size& size, Scale& scale,
		   PlayerMoveHistory& playerMoveHistory)
		{
			translation.value.x = PLAYER_START_X;
			size.value = float2(PADDLE_DEFAULT_SIZE_X, PADDLE_DEFAULT_SIZE_Y);
			move.state = 0;
			move.direction = float2(0, 0);
			move.speed = PLAYER_DEFAULT_SPEED;
			scale.value = float2(1, 1);
			playerMoveHistory.queue.clear();
		});

	entities->each<Ball, Translation, Move, Size, Scale>(
		[](ex::Entity entity, Ball& ball, Translation& translation, Move& move, Size& size, Scale& scale)
		{
			translation.value = float2(BALL_START_X, BALL_START_Y);
			size.value = float2(BALL_RADIUS * 2.f, BALL_RADIUS * 2.f);
			move.state = 0;
			move.direction = float2(0.0f, -1.0f);
			move.speed = BALL_DEFAULT_SPEED;
			scale.value = float2(1, 1);
		});
}


#endif
