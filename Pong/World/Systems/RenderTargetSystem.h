#pragma once

#include <entityx/entityx.h>


namespace ex = entityx;

class RenderTargetSystem : public ex::System<RenderTargetSystem>
{
public:
	/// Calculates render targets sizes and positions. Note that position of middle of the screen is (0,0)
	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;
};


