#pragma once
#include <entityx/Entity.h>
#include "../Util/Constants.h"
#include "World.h"

#include "Components/RectangleShape.h"
#include "Components/Translation.h"
#include "Components/Scale.h"
#include "Components/Size.h"
#include "Components/RenderOrder.h"
#include "Components/RenderTarget.h"
#include "Components/Name.h"
#include "Components/CircleShape.h"
#include "Components/Image.h"
#include "Components/Move.h"
#include "Components/Rival.h"
#include "Components/Player.h"
#include "Components/Ball.h"
#include "Components/Collider.h"
#include "Components/Text.h"
#include "Components/PlayerMoveHistory.h"
#include "Components/PlayerNetworkInfo.h"
#include "Components/Paddle.h"


using namespace linalg::aliases;

/// This class creates entities and assigns them to their default values.
class EntityFactory
{
public:
	
	/// Create Backgorund entity with its components.
	static Entity CreateBackground()
	{
		SpriteType asset = SpriteType::Background;
		auto e = World::Instance.entities.create();
		e.assign<Name>(asset._to_string());
		e.assign<Translation>(float2{0, 0});
		e.assign<Scale>();
		e.assign<Size>(float2{CLIENT_WINDOW_WIDTH, CLIENT_WINDOW_HEIGHT});
		e.assign<RenderOrder>(0);
		e.assign<RenderTarget>();
		e.assign<RectangleShape>(Color{1, 0, 0}, false);
		e.assign<Image>(asset, false);

		return e;
	}

	/// Create Paddle entity with its components and default values.
	static void AddPaddle(Entity e, bool isRival)
	{
		SpriteType asset = SpriteType::Paddle;

		e.assign<Paddle>();
		e.assign<Translation>(float2{ PLAYER_START_X, isRival ? PLAYER_START_Y : -PLAYER_START_Y });
		e.assign<Scale>();
		e.assign<Size>(float2{ PADDLE_DEFAULT_SIZE_X, PADDLE_DEFAULT_SIZE_Y });
		e.assign<RenderOrder>(1);
		e.assign<RenderTarget>();
		e.assign<Move>(PLAYER_DEFAULT_SPEED, float2(0, 0));
		e.assign<RectangleShape>(Color{ 1, 0, 0 }, false);
		e.assign<Image>(asset, false);
		e.assign<Collider>(ColliderType::Rectangle, float2(0, isRival ? -1 : 1));
		e.assign<PlayerMoveHistory>();
	}

	/// Create Player entity with its components and default values.
	static Entity CreatePlayer(bool isRival)
	{
		auto e = World::Instance.entities.create();
		e.assign<Name>(isRival ? "Rival" : "Player");
		e.assign<PlayerNetworkInfo>();
		if (isRival)
		{
			e.assign<NetworkObjectType>(NetworkObjectType::Server);
			e.assign<Rival>(1);
		}
		else
		{
			e.assign<NetworkObjectType>(NetworkObjectType::Client);
			e.assign<Player>(0);
		}

		return e;
	}

	/// Create Ball entity with its components and default values.
	static Entity CreateBall()
	{
		SpriteType asset = SpriteType::Ball;
		auto e = World::Instance.entities.create();
		e.assign<Name>(asset._to_string());
		e.assign<Translation>(float2{BALL_START_X, BALL_START_Y});
		e.assign<Scale>();
		e.assign<Size>(float2{BALL_RADIUS * 2.f, BALL_RADIUS * 2.f});
		e.assign<RenderOrder>(2);
		e.assign<RenderTarget>();
		e.assign<Move>(BALL_DEFAULT_SPEED, float2(0.0f, -1.0f));
		e.assign<CircleShape>(BALL_RADIUS, Color{0, 1, 0}, false);
		e.assign<Image>(asset, false);
		e.assign<Collider>(ColliderType::Circle);
		e.assign<Ball>(3);
		e.assign<NetworkObjectType>(NetworkObjectType::Server);

		return e;
	}

	/// Create Side Walls entity with its components and default values.
	static Entity CreateSideWalls(bool left)
	{
		const auto rightEdge = CLIENT_WINDOW_WIDTH * 0.5f;
		const auto leftEdge = -CLIENT_WINDOW_WIDTH * 0.5f;
		
		SpriteType asset = SpriteType::Paddle;
		auto e = World::Instance.entities.create();
		e.assign<Name>(left ? "Wall_Left" : "Wall_Right");
		const auto x = left ? leftEdge + SIDE_WALL_SIZE_X * 0.5f : rightEdge - SIDE_WALL_SIZE_X * 0.5f;
		e.assign<Translation>(float2{ x, 0 });
		e.assign<Scale>();
		e.assign<Size>(float2{ SIDE_WALL_SIZE_X, SIDE_WALL_SIZE_Y });
		e.assign<RenderOrder>(0);
		e.assign<RenderTarget>();
		e.assign<RectangleShape>(Color{ 0, 0, 1 }, true);
		e.assign<Collider>(ColliderType::Rectangle, float2(left ? 1 : -1, 0));

		return e;
	}

	/// Create Text entity with its components and default values.
	static Entity CreateText(const TextId& textId, const char* text, const FontType fontType, int fontSize, const float2& pos, const Color& color, const float& scale = 1)
	{
		auto e = World::Instance.entities.create();
		e.assign<Name>("Text");
		e.assign<Translation>(pos);
		e.assign<Scale>(float2{scale, scale});
		e.assign<Text>(textId, text, fontType, fontSize, color);

		return e;
	}


};
