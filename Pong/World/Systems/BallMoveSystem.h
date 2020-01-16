#pragma once

#include <entityx/System.h>


struct UpdateBallMoveEvent;
struct CollisionEvent;
namespace ex = entityx;

class BallMoveSystem : public ex::System<BallMoveSystem>, public ex::Receiver<BallMoveSystem>
{
public:
	BallMoveSystem();
	
	/// Configures the system and subscribes to CollisionEvent, UpdateBallMoveEvent
	void configure(entityx::EntityManager& entities, entityx::EventManager& events) override;
	/// Updates ball movements and increases ball move state.
	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;

	/// CollisionEvent receiver. Detects collisions and decides new ball's position and direction.
	void receive(const CollisionEvent& event);
	/// UpdateBallMoveEvent receiver. Gets server messages and corrects ball respect to these messages.
	/// This part works for  only client.
	void receive(const UpdateBallMoveEvent& event);

private:
	/// Calculates new direction of ball. Since ball is server owner object we don't need to send any
	/// information about ball to server. We only have to obey the servers commands and interpolate it.
	void CalculateBallDirection(entityx::Entity& ball, entityx::Entity& other) const;
	
	entityx::EntityManager* entities;
	entityx::EventManager* events;
};
