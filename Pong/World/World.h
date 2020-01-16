#pragma once
#include "entityx/entityx.h"
#include <SDL2/SDL.h>
#include <atomic>

using namespace entityx;

class World : public EntityX
{
public:
	static World Instance;

	/// Starts world and adds all systems to update in future.
	explicit World();

	/// Preparing world to spin, creating some default entities.
	void PrepareWorld();
	/// This function is the main loop of the game it updates logic, renders the game.
	void Spin();
	/// Pauses the world.
	void PauseWorld(bool value);
	/// Getter for pause.
	bool IsWorldPaused() const;

	bool DidBigBangHappen() const;

private:
	std::atomic<bool> didBigBangHappen;
	std::atomic<bool> pause;

	/// Updates all systems, our update order is fetching network messages
/// then update logic after all render all entities, of course if there
/// is any entity that marked as destroyed we are destroying them.
	void Update(const double& delta);

	/// Removes the world.
	void BringDoom();

	/// Renders the game if graphics is ready or activated.
	/// This part does not work on server, because server is designed to be headless
	/// so that it does not need any graphics engine. See the flag in `GraphicsManager::Instance.Ready()`
	void OnRender(const float& delta);

	/// Polls events.
	void OnEvent(SDL_Event* event);
};
