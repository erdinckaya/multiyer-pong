#pragma once

#include <entityx/System.h>


namespace ex = entityx;

class BotSystem : public ex::System<BotSystem>
{
public:
	/// Fetches bots and make them follow the ball.
	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;
};


