#include "BotSystem.h"
#include "../Components/Bot.h"
#include "../Components/Translation.h"
#include "../Components/Ball.h"

void BotSystem::update(ex::EntityManager& entities, ex::EventManager& events, ex::TimeDelta dt)
{
	// For every bots.
	entities.each<Bot, Translation>([&entities](ex::Entity botEntity, Bot& bot, Translation& botTranslation)
	{
		// Return if it is not enabled. 
		if (!bot.enabled)
		{
			return;
		}

		// Get ball.
		entities.each<Ball, Translation>([&botTranslation](ex::Entity ballEntity, Ball& ball, Translation& ballTranslation)
		{
			// follow the ball.
			botTranslation.value.x = ballTranslation.value.x;
		});
	});
}
