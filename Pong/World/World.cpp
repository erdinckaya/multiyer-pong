#include "World.h"
#include "../Graphics/GraphicsManager.h"
#include "Systems/RenderTargetSystem.h"
#include "Systems/RenderShapeSystem.h"
#include "EntityFactory.h"
#include "Systems/RenderImageSystem.h"
#include "Systems/SizeSystem.h"
#include "../Input/MouseInput.h"
#include "../Input/KeyInput.h"
#include "Systems/PlayerMoveSystem.h"
#include "Systems/BallMoveSystem.h"
#include "Systems/CollisionSystem.h"
#include "Systems/GizmoSystem.h"
#include "Systems/DestroySystem.h"
#include "Systems/TextSystem.h"

#include "Systems/NetworkMessageSystem.h"
#include "Systems/BotSystem.h"
#include "Events/ChangeTextEvent.h"


#ifdef CLIENT
#include "Systems/ClientGameManagerSystem.h"
#else
#include "Systems/ServerGameManagerSystem.h"
#endif



World World::Instance;

World::World() : didBigBangHappen(true), pause(false)
{
	// Adding systems order is not matter.
	
#ifdef CLIENT
	systems.add<ClientGameManagerSystem>();
#else
	systems.add<ServerGameManagerSystem>();
#endif
	
	systems.add<NetworkMessageSystem>();
	systems.add<BallMoveSystem>();
	systems.add<BotSystem>();
	systems.add<PlayerMoveSystem>();
	systems.add<CollisionSystem>();
	systems.add<SizeSystem>();
	systems.add<GizmoSystem>();
	systems.add<RenderTargetSystem>();
	systems.add<RenderImageSystem>();
	systems.add<TextSystem>();

	systems.add<RenderShapeSystem>();

	systems.add<DestroySystem>();
}


void World::Update(const double& delta)
{
	// Network
	systems.update<NetworkMessageSystem>(delta);

	// When pause activated it only affects logic part.
	if (!IsWorldPaused())
	{
		// Logic
		systems.update<PlayerMoveSystem>(delta);
		systems.update<BallMoveSystem>(delta);
		systems.update<BotSystem>(delta);
		systems.update<CollisionSystem>(delta);
		systems.update<SizeSystem>(delta);
	}
#ifdef CLIENT
	systems.update<ClientGameManagerSystem>(delta);
#else
	systems.update<ServerGameManagerSystem>(delta);
#endif

	// Render
	systems.update<GizmoSystem>(delta);
	systems.update<RenderTargetSystem>(delta);
	systems.update<RenderImageSystem>(delta);
	systems.update<RenderShapeSystem>(delta);
	systems.update<TextSystem>(delta);

	// Destroy
	systems.update<DestroySystem>(delta);
}


void World::PrepareWorld()
{
	// Default game entities.
	EntityFactory::CreateBackground();
	EntityFactory::CreatePlayer(true);
	EntityFactory::CreatePlayer(false);
	EntityFactory::CreateSideWalls(true);
	EntityFactory::CreateSideWalls(false);


	// Creating default text entities.
	const int fontSize = 25;
	const float yTopOffset = CLIENT_WINDOW_HEIGHT * 0.5f;
	float2 leftTop = {-CLIENT_WINDOW_WIDTH * 0.5f, yTopOffset};
	EntityFactory::CreateText(TextId::Fps, "FPS: 0", FontType::Fontana, fontSize, leftTop, Color{1, 1, 0, 1});
	leftTop.y -= fontSize;
	EntityFactory::CreateText(TextId::Pause, "Pause: false", FontType::Fontana, fontSize, leftTop, Color{ 1, 1, 0, 1 });
	leftTop.y -= fontSize;
	EntityFactory::CreateText(TextId::HelpTip, "Help: H", FontType::Fontana, fontSize, leftTop, Color{ 1, 1, 0, 1 });
}


void World::Spin()
{
	// Configure systems.
	systems.configure();
	// Prepare world to spin.
	PrepareWorld();
	SDL_Event event{};
	// Get current time.
	Uint32 time = SDL_GetTicks();
	// If big bang happened, than spin.
	while (didBigBangHappen)
	{
		// Fetch user events by using SDL2.
		const Uint32 currentTime = SDL_GetTicks();
		while (SDL_PollEvent(&event))
		{
			OnEvent(&event);
		}

		// Check the elapsed time.
		if (time <= currentTime)
		{
			// Calculate FPS.
			const double dt = (currentTime - time + FIXED_GAME_DELTA * 1000) * 0.001;
			// Set fps text to render.
			events.emit<ChangeTextEvent>(TextId::Fps, "FPS: " + std::to_string(static_cast<Uint32>(1.0 / dt)));
			Update(dt);

			// Advance time.
			time = currentTime + static_cast<Uint32>(FIXED_GAME_DELTA * 1000.0);

			// Render entities.
			OnRender(dt);

			// Clear input buffers.
			KeyInput::Instance.BeginNewFrame();
			MouseInput::Instance.BeginNewFrame();
		}
		else
		{
			// Sleep 1 ms. Dont sleep exact difference between you current time and next fps time, because operating system
			// cannot awake you in time and that causes glitches in your game.
			SDL_Delay(1);
		}
	}
	
	// Bring the doom to erase all livings.
	BringDoom();
}


void World::PauseWorld(bool value)
{
	pause = value;
}


bool World::IsWorldPaused() const
{
	return pause;
}

bool World::DidBigBangHappen() const
{
	return didBigBangHappen;
}


void World::OnEvent(SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_MOUSEBUTTONDOWN:
		{
			MouseInput::Instance.MouseDownEvent(*event);
			break;
		}
	case SDL_MOUSEBUTTONUP:
		{
			MouseInput::Instance.MouseUpEvent(*event);
			break;
		}
	case SDL_KEYDOWN:
		{
			KeyInput::Instance.KeyDownEvent(*event);
			break;
		}
	case SDL_KEYUP:
		{
			KeyInput::Instance.KeyUpEvent(*event);
			break;
		}
	case SDL_QUIT:
		{
			BringDoom();
			break;
		}
	default:
		break;
	}
}

/// Removes the world.
void World::BringDoom()
{
	didBigBangHappen = false;
	
	SDL_Quit();
}

void World::OnRender(const float& delta)
{
	// Checks graphics.
	if (GraphicsManager::Instance.Ready())
	{
		// Render all entities.
		GraphicsManager::Instance.GetGraphics()->Update(delta);
	}
}
