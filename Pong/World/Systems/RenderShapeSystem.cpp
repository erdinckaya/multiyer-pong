#include "RenderShapeSystem.h"



#include "../Components/RectangleShape.h"
#include "../Components/RenderOrder.h"
#include "../../Graphics/GraphicsManager.h"
#include "../Components/RenderTarget.h"
#include "../Components/CircleShape.h"
#include "../../Util/UtilMacro.h"

void RenderShapeSystem::update(ex::EntityManager& entities, ex::EventManager& events, ex::TimeDelta dt)
{
	// Check graphics.
	if (!GraphicsManager::Instance.Ready())
	{
#ifdef CLIENT
		DEBUG_CLIENT(info, "Graphics is not ready!");
#endif
		return;
		}

	auto graphics = GraphicsManager::Instance.GetGraphics();

	// Get all render targets with order.
	std::vector<ex::Entity> renderTargets;
	entities.each<RenderTarget, RenderOrder>(
		[&renderTargets](ex::Entity entity, RenderTarget& renderTarget, RenderOrder& renderOrder)
		{
			renderTargets.emplace_back(entity);
		});
	// Sort them all.
	std::sort(renderTargets.begin(), renderTargets.end(), [](ex::Entity& a, ex::Entity& b) -> bool
	{
		const int aVal = a.component<RenderOrder>()->value;
		const int bVal = b.component<RenderOrder>()->value;
		return aVal < bVal;
	});

	// Render them according to their shape style.
	for (auto& target : renderTargets)
	{
		auto renderTarget = target.component<RenderTarget>();

		// Render rectangles.
		if (target.has_component<RectangleShape>())
		{
			auto shape = target.component<RectangleShape>();
			if (shape->render)
			{
				graphics->DrawRect(renderTarget->value, shape->color, shape->lineWidth, shape->fill);
			}
		}
		else if (target.has_component<CircleShape>()) // Render circles.
		{
			auto shape = target.component<CircleShape>();
			if (shape->render)
			{
				// Calculate center of circle and radius.
				const float2 center(renderTarget->value.x + renderTarget->value.w * 0.5f, renderTarget->value.y + renderTarget->value.z * 0.5f);
				const float radius = renderTarget->value.w * 0.5f;
				graphics->DrawCircle(center, radius, shape->color, shape->lineWidth, shape->fill);
			}
		}
	}
}
