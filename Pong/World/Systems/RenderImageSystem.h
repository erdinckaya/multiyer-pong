#pragma once

#include <entityx/entityx.h>


namespace ex = entityx;

class RenderImageSystem : public ex::System<RenderImageSystem>
{
public:
	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;
};


