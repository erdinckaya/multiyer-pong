#pragma once
#include <entityx/entityx.h>

namespace ex = entityx;

struct CollisionEvent
{
	explicit CollisionEvent(const ex::Entity& e1, const ex::Entity e2) : e1(e1), e2(e2)
	{
	}

	ex::Entity e1;
	ex::Entity e2;
};
