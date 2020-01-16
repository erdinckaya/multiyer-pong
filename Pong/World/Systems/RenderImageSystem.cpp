
#include "RenderImageSystem.h"


#include "../Components/RenderOrder.h"
#include "../../Graphics/GraphicsManager.h"
#include "../Components/RenderTarget.h"
#include "../Components/Image.h"
#include "../../Util/AssetManager.h"
#include "../../Util/UtilMacro.h"

/// Renders all images respect to their render orders.
void RenderImageSystem::update(ex::EntityManager& entities, ex::EventManager& events, ex::TimeDelta dt)
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

	// Fetch all images.
	std::vector<ex::Entity> renderTargets;
	entities.each<RenderTarget, RenderOrder, Image>(
		[&renderTargets](ex::Entity entity, RenderTarget& renderTarget, RenderOrder& renderOrder, Image& image)
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

	// Render all images.
	for (auto& target : renderTargets)
	{
		auto renderTarget = target.component<RenderTarget>();
		auto image = target.component<Image>();

		graphics->DrawImage(AssetManager::Instance[image->value], renderTarget->value, 0, false, false);
	}
}
