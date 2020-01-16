#pragma once

#include <entityx/entityx.h>


namespace ex = entityx;

class GizmoSystem : public ex::System<GizmoSystem>
{
public:
	// Draws gizmoes namely borders of colliders.
	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;
};


