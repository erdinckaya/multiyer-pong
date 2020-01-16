#pragma once

#include <yojimbo.h>
// Game constants.


constexpr uint64_t PROTOCOL_ID = 0x11223344556677UL;
constexpr uint16_t SERVER_PORT = 40000;
constexpr const char* SERVER_ADDRESS = "127.0.0.1";

constexpr uint8_t DEFAULT_PRIVATE_KEY[yojimbo::KeyBytes] = {
	0x60, 0x6a, 0xbe, 0x6e, 0xc9, 0x19, 0x10, 0xea,
	0x9a, 0x65, 0x62, 0xf6, 0x6f, 0x2b, 0x30, 0xe4,
	0x43, 0x71, 0xd6, 0x2c, 0xd1, 0x99, 0x27, 0x26,
	0x6b, 0x3c, 0x60, 0xf4, 0xb7, 0x15, 0xab, 0xa1
};


constexpr float CLIENT_WINDOW_WIDTH = 800.0f;
constexpr float CLIENT_WINDOW_HEIGHT = 600.0f;
constexpr const char* CLIENT_WINDOW_TITLE = "CLIENT";


constexpr int FONT_SIZE_MAX = 100;


constexpr float FIXED_SERVER_DELTA = 1.0f / 60.0f;
constexpr float FIXED_GAME_DELTA = 1.0 / 60.0;

constexpr float PLAYER_START_X = 0.f;
constexpr float PLAYER_START_Y = 250.f;

constexpr float BALL_START_X = 0.f;
constexpr float BALL_START_Y = 0.f;
constexpr float BALL_RADIUS = 15.f;


constexpr float PADDLE_DEFAULT_SIZE_X = 150.0f;
constexpr float PADDLE_DEFAULT_SIZE_Y = 20.0f;

constexpr float SIDE_WALL_SIZE_X = 5.0f;
constexpr float SIDE_WALL_SIZE_Y = 600.0f;

constexpr float PLAYER_DEFAULT_SPEED = 750;
constexpr float BALL_DEFAULT_SPEED = 400;


constexpr int PLAYER_MOVE_HISTORY_SIZE = 20;

constexpr int GAME_PLAYER_MAX = 2;


constexpr int PLAYER_POSITION_HISTORY_SIZE = 50;


constexpr int BALL_MOVE_STATE_TOLERANCE = 6;
constexpr int PLAYER_MOVE_STATE_TOLERANCE = 4;


constexpr double PI = 3.14159265;
constexpr float MAX_BALL_COLLIDE_ANGLE = 75;
