#pragma once

#include <entityx/entityx.h>

namespace ex = entityx;

class SizeSystem : public ex::System<SizeSystem>
{
public:
	/// Updates all entities which have size and image components.
	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;
};


