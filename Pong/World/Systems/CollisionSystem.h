#pragma once

#include <entityx/System.h>




namespace ex = entityx;

class CollisionSystem : public ex::System<CollisionSystem>
{
public:
	/// Checks collisions of colliders.
	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;
};


