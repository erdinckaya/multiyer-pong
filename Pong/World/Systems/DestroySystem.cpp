#include "DestroySystem.h"

#include "../Components/Destroy.h"

void DestroySystem::update(ex::EntityManager& entities, ex::EventManager& events, ex::TimeDelta dt)
{
	entities.each<Destroy>([](ex::Entity entity, Destroy& destroy)
	{
		if (entity.valid())
		{
			entity.destroy();
		}
	});
}
