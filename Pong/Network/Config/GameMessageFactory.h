#pragma once

#include "GameMessageType.h"
#include "../Data/MyMessage.h"
#include "../Data/GameStateMessage.h"
#include "../Data/ClientConnectedMessage.h"
#include "../Data/StartGameMessage.h"
#include "../Data/PlayerMoveMessage.h"
#include "../Data/PauseCommandMessage.h"
#include "../Data/RestartCommandMessage.h"

YOJIMBO_MESSAGE_FACTORY_START(GameMessageFactory, static_cast<int>(GameMessageType::COUNT))
	YOJIMBO_DECLARE_MESSAGE_TYPE((int) GameMessageType::TEST, MyMessage)
	YOJIMBO_DECLARE_MESSAGE_TYPE((int) GameMessageType::CLIENT_CONNECTED, ClientConnectedMessage)
	YOJIMBO_DECLARE_MESSAGE_TYPE((int) GameMessageType::START_GAME, StartGameMessage)
	YOJIMBO_DECLARE_MESSAGE_TYPE((int) GameMessageType::GAME_STATE, GameStateMessage)
	YOJIMBO_DECLARE_MESSAGE_TYPE((int) GameMessageType::PLAYER_MOVE, PlayerMoveMessage)
	YOJIMBO_DECLARE_MESSAGE_TYPE((int) GameMessageType::PAUSE, PauseCommandMessage)
	YOJIMBO_DECLARE_MESSAGE_TYPE((int) GameMessageType::RESTART, RestartCommandMessage)
YOJIMBO_MESSAGE_FACTORY_FINISH()
