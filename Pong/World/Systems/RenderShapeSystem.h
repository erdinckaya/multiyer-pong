#pragma once

#include <entityx/entityx.h>


namespace ex = entityx;

class RenderShapeSystem : public ex::System<RenderShapeSystem>
{
public:
	/// Fetches all render targets and render them all respect to their render order.
	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;
};


