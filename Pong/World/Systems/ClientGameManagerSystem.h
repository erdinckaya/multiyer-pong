#pragma once

#ifdef CLIENT
#include <entityx/entityx.h>



struct RestartEvent;
struct PauseEvent;
struct AddPlayerEvent;
struct StartGameEvent;

namespace ex = entityx;

class ClientGameManagerSystem : public ex::System<ClientGameManagerSystem>, public ex::Receiver<ClientGameManagerSystem>
{
public:

	ClientGameManagerSystem();

	/// Handles users actions apart from game, such as pause, help menu or network debugging.
	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;

	/// Configures the system and subscribes to AddPlayerEvent, StartGameEvent, PauseEvent, RestartEvent
	void configure(entityx::EntityManager& entities, entityx::EventManager& events) override;
	
	/// AddPlayerEvent receiver. It sets game window title and creates players's components and activate it.
	void receive(const AddPlayerEvent& addPlayerEvent);

	/// StartGameEvent receiver. Creates ball and network panel. Game starts.
	void receive(const StartGameEvent& startGameEvent);
	
	/// PauseEvent receiver. Updates pause text and pauses the game.
	void receive(const PauseEvent& pauseEvent);
	
	/// RestartEvent receiver. Assigns all entities to their default. 
	void receive(const RestartEvent& restartEvent);

private:
	/// By assigning all entities to default values, restarts game.
	void Restart() const;

	/// Checks pause and restart key, if there is sends server related command.
	static void HandlePauseAndRestart();
	
	/// Opens or closes help menu.
	void HandleHelpMenu();

	/// Creates network panel.
	void CreateNetworkPanel();

	/// Updates network panel by fetching network infoes from NetworkManager.
	void UpdateNetworkPanel() const;

	/// Handles user inputs for debugging network.
	void DebugNetwork() const;

	// panel flags
	bool isHelpMenuOpen;
	bool isNetworkMenuOpened;

	entityx::EntityManager* entities;
	entityx::EventManager* events;
};

#endif