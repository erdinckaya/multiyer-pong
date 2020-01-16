#include "CollisionSystem.h"

#include "../Components/Collider.h"
#include "../../Util/IntersectionUtil.h"
#include "../Components/Translation.h"
#include "../Components/RenderTarget.h"
#include "../Components/CircleShape.h"
#include "../Events/CollisionEvent.h"

void CollisionSystem::update(ex::EntityManager& entities, ex::EventManager& events, ex::TimeDelta dt)
{
	// Get every collider.
	std::vector<ex::Entity> entitySet;
	entities.each<Collider>([&entitySet](ex::Entity entity, Collider& collider)
	{
		entitySet.push_back(entity);
	});

	// Create a collision table. and check every collider.
	const int size = entitySet.size();
	std::vector<std::vector<int>> collisions(size, std::vector<int>(size, false));
	for (int i = 0; i < size; i++)
	{
		auto e1 = entitySet[i];
		for (int j = 0; j < size; j++)
		{
			auto e2 = entitySet[j];
			if (i == j || collisions[j][i])
			{
				continue;
			}


			// Check for rectangle to rectangle
			bool intersect = false;
			auto c1 = e1.component<Collider>();
			auto c2 = e2.component<Collider>();
			if (c1->type == +ColliderType::Rectangle && c2->type == +ColliderType::Rectangle)
			{
				// Check rectangle to rectangle intersection.
				const auto rect1 = e1.component<RenderTarget>()->value;
				const auto rect2 = e2.component<RenderTarget>()->value;
				float4 rec;
				intersect = IntersectionUtil::RectToRect(rect1, rect2, rec);
			}
			// Check for circle to rectangle
			else if (c1->type == +ColliderType::Circle && c2->type == +ColliderType::Rectangle)
			{
				// Check circle to rectangle intersection.
				const auto r = e1.component<CircleShape>()->radius;
				const auto c = e1.component<Translation>()->value;
				const auto target = e2.component<RenderTarget>()->value;
				const auto translation = e2.component<Translation>()->value;
				const float4 rect = float4(translation.x, translation.y, target.z, target.w);
				intersect = IntersectionUtil::CircleToRect(c, r, rect);
			}
			// Check for rectangle to circle
			else if (c1->type == +ColliderType::Rectangle && c2->type == +ColliderType::Circle)
			{
				// Check circle to rectangle intersection.
				const auto r = e2.component<CircleShape>()->radius;
				const auto c = e2.component<Translation>()->value;
				const auto target = e1.component<RenderTarget>()->value;
				const auto translation = e1.component<Translation>()->value;
				const float4 rect = float4(translation.x, translation.y, target.z, target.w);
				intersect = IntersectionUtil::CircleToRect(c, r, rect);
			}
			// Check for circle to circle
			else if (c1->type == +ColliderType::Circle && c2->type == +ColliderType::Circle)
			{
				// Check circle to circle intersection.
				const auto r1 = e1.component<CircleShape>()->radius;
				const auto c1 = e1.component<Translation>()->value;
				const auto r2 = e2.component<CircleShape>()->radius;
				const auto c2 = e2.component<Translation>()->value;
				intersect = IntersectionUtil::CircleToCircle(c1, r1, c2, r2);
			}
			
			if (intersect)
			{
				// Notify them all.
				events.emit<CollisionEvent>(e1, e2);
			}

			// Fill the collision table.
			collisions[i][j] = intersect;
		}
	}
}
