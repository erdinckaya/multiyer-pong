#include "GizmoSystem.h"
#include "../../Input/KeyInput.h"

#include "../Components/Collider.h"
#include "../../Graphics/GraphicsManager.h"
#include "../Components/RectangleShape.h"
#include "../Components/CircleShape.h"
#include "../../Util/UtilMacro.h"

void GizmoSystem::update(ex::EntityManager& entities, ex::EventManager& events, ex::TimeDelta dt)
{
	// Check graphics.
	if (!GraphicsManager::Instance.Ready())
	{
#ifndef SERVER
		DEBUG_CLIENT(info, "Graphics is not ready!");
#endif
		return;
	}

	// For every colliders.
	entities.each<Collider>([](ex::Entity entity, Collider& collider)
	{
		// Set render flag.
		const auto value = KeyInput::Instance.IsKeyHeld(SDL_SCANCODE_C);
		if (collider.type == +ColliderType::Rectangle)
		{
			entity.component<RectangleShape>()->render = value;
		}
		else if (collider.type == +ColliderType::Circle)
		{
			entity.component<CircleShape>()->render = value;
		}
	});
}
