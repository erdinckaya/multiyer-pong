#include "RenderTargetSystem.h"


#include "../Components/Translation.h"
#include "../Components/Scale.h"
#include "../Components/Size.h"
#include "../Components/RenderTarget.h"
#include "../Components/RectangleShape.h"
#include "../Components/CircleShape.h"

using namespace linalg::aliases;

void RenderTargetSystem::update(ex::EntityManager& entities, ex::EventManager& events, ex::TimeDelta dt)
{
	// Get rectangle render targets.
	entities.each<RenderTarget, Translation, Scale, Size, RectangleShape>(
		[&](ex::Entity entity, RenderTarget& renderTarget, Translation& translation, Scale& scale, Size& size,
		    RectangleShape& rectangleShape)
		{
			// Calculate their sizes and positions.
			const float2 rectSize = size.value * scale.value;
			const float2 leftTop = translation.value - (size.value * 0.5f * scale.value);
			renderTarget.value.x = leftTop.x;
			renderTarget.value.y = leftTop.y;
			renderTarget.value.w = rectSize.x;
			renderTarget.value.z = rectSize.y;
		});

	// Get circle render targets.
	entities.each<RenderTarget, Translation, Scale, CircleShape>(
		[&](ex::Entity entity, RenderTarget& renderTarget, Translation& translation, Scale& scale,
		    CircleShape& circleShape)
		{
		// Calculate their sizes and positions.
			auto R = circleShape.radius * 2.f;
			const float2 rectSize = R * scale.value;
			const float2 leftTop = translation.value - (rectSize * 0.5f);
			renderTarget.value.x = leftTop.x;
			renderTarget.value.y = leftTop.y;
			renderTarget.value.w = rectSize.x;
			renderTarget.value.z = rectSize.y;
		});
}
