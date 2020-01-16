#include "BallMoveSystem.h"
#include "../Components/Translation.h"
#include "../Components/Size.h"
#include "../Components/Ball.h"
#include "../Components/CircleShape.h"
#include "../../Util/Constants.h"
#include "../Components/Move.h"
#include "../Components/Collider.h"
#include "../Components/RenderTarget.h"
#include "../Events/CollisionEvent.h"
#include "../Events/UpdateBallMoveEvent.h"
#include "../../Util/UtilMacro.h"
#include "../Components/Paddle.h"


BallMoveSystem::BallMoveSystem() : entities(nullptr), events(nullptr)
{
}

void BallMoveSystem::configure(entityx::EntityManager& entities, entityx::EventManager& events)
{
	this->entities = &entities;
	this->events = &events;
	events.subscribe<CollisionEvent>(*this);
	events.subscribe<UpdateBallMoveEvent>(*this);
}

void BallMoveSystem::update(ex::EntityManager& entities, ex::EventManager& events, ex::TimeDelta dt)
{
	// Fetch every ball, in our case we have one ball.
	entities.each<Move, Translation, Size, Ball, CircleShape>(
		[&dt](ex::Entity entity, Move& move, Translation& translation, Size& size, Ball& ball, CircleShape& shape)
		{
			// Advance ball
			translation.value += move.speed * static_cast<float>(dt) * move.direction;
			move.state++;

			// Check for wall boundries, correct it, namely prevent them to start from inside of walls.
			const auto left = -CLIENT_WINDOW_WIDTH * 0.5f;
			const auto right = CLIENT_WINDOW_WIDTH * 0.5f;
			const auto leftEdge = translation.value.x - shape.radius;
			const auto rightEdge = translation.value.x + shape.radius;
			if (leftEdge <= left)
			{
				translation.value.x = left + shape.radius + 1;
			}
			if (rightEdge >= right)
			{
				translation.value.x = right - shape.radius - 1;
			}
		});
}

void BallMoveSystem::receive(const CollisionEvent& collisionEvent)
{
	CollisionEvent event = collisionEvent;
	// Return none of them is ball. Because we are only caring ball collisions, in this system.
	if (!event.e1.has_component<Ball>() && !event.e2.has_component<Ball>())
	{
		return;
	}

	auto ball = event.e1.has_component<Ball>() ? event.e1 : event.e2;
	auto other = !event.e1.has_component<Ball>() ? event.e1 : event.e2;

	// Calculate new position. 
	const auto radius = ball.component<CircleShape>()->radius;
	auto translation = ball.component<Translation>();
	const auto front = other.component<Collider>()->front;
	const auto rect = other.component<RenderTarget>()->value;
	const auto center = other.component<Translation>()->value;
	const auto diffX = radius + rect.w * 0.5f - std::abs(translation->value.x - center.x);
	const auto diffY = radius + rect.z * 0.5f - std::abs(translation->value.y - center.y);

	translation->value.x += diffX * front.x;
	translation->value.y += diffY * front.y;

	// Calculate new position
	CalculateBallDirection(ball, other);
}


void BallMoveSystem::receive(const UpdateBallMoveEvent& event)
{
#ifdef CLIENT
	entities->each<Ball, Move, Translation>(
		[&event](ex::Entity entity, Ball& ball, Move& move, Translation& translation)
		{
			// Assign directions and speed.
			move.direction = event.direction;
			move.speed = event.speed;

			// Calculate the tolerance and check it, if ball is too ahead from server correct it.
			const auto distanceTolerance = move.speed * FIXED_GAME_DELTA * BALL_MOVE_STATE_TOLERANCE;
			const auto distanceToleranceSquared = distanceTolerance * distanceTolerance;
			const auto distanceSquared = linalg::distance2(event.pos, translation.value);
			const auto canSync = distanceSquared >= distanceToleranceSquared;
			DEBUG_COND(canSync, "Distance: {0} tolenrance: {1}", distanceSquared, distanceToleranceSquared);
			if (canSync)
			{
				DEBUG_LOG("Move state {0} event state {1}", move.state, event.state);
				translation.value = event.pos;
				move.state = event.state;
			}
		});
#endif
}


void BallMoveSystem::CalculateBallDirection(entityx::Entity& ball, entityx::Entity& other) const
{
	// Check for entity validation.
	if (!ball.valid() || !other.valid())
	{
		DEBUG_LOG("One of the collider entity is not valid!");
		return;
	}

	const auto isPaddle = other.has_component<Paddle>();
	const auto front = other.component<Collider>()->front;
	if (isPaddle)
	{
		// Calculate ball collision point with paddle.
		const auto paddleWidth = other.component<Size>()->value.x * 0.5f;
		const auto paddleCenter = other.component<Translation>()->value.x;
		const auto ballCenter = ball.component<Translation>()->value.x;
		// Calculate ratio of collision point respect to paddle center.s
		const auto ratio = (ballCenter - paddleCenter) / paddleWidth;
		// Find the new angle.
		const auto angle = ratio * MAX_BALL_COLLIDE_ANGLE;
		const auto radians = DEG2RAD(angle);
		// Create normalize vector for new direction.
		const auto direction = float2(std::sin(radians), std::cos(radians) * front.y);
		
		ball.component<Move>()->direction = direction;
	}
	else // Wall.
	{
		// Just return back according to front axis.
		if (front.x != 0)
		{
			ball.component<Move>()->direction.x = front.x;
		}
		if (front.y != 0)
		{
			ball.component<Move>()->direction.y = front.y;
		}
	}
}
