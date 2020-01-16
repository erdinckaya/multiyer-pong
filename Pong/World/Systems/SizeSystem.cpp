#include "SizeSystem.h"

#include "../../Util/AssetManager.h"

#include "../Components/Image.h"
#include "../Components/Size.h"


void SizeSystem::update(ex::EntityManager& entities, ex::EventManager& events, ex::TimeDelta dt)
{
	entities.each<Size, Image>([](ex::Entity entity, Size& size, Image& image)
	{
		// Force to get real size.
		if (image.realSize)
		{
			size.value.x = AssetManager::Instance[image.value]->Width();
			size.value.y = AssetManager::Instance[image.value]->Height();
		}
	});
}
