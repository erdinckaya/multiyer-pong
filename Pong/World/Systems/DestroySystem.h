#pragma once

#include <entityx/System.h>




namespace ex = entityx;

class DestroySystem : public ex::System<DestroySystem>
{
public:
	/// Fetches all marked components as destroy then destroys them. This system runs after all systems.
	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;
};


