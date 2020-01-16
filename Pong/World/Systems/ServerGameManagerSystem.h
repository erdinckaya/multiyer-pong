#pragma once

#ifdef SERVER

#include <entityx/entityx.h>



#ifdef SendMessage
#undef SendMessage
#endif

struct AddPlayerEvent;
struct StartGameEvent;
struct SendGameStateEvent;
struct RestartEvent;
struct PauseEvent;
namespace ex = entityx;

class ServerGameManagerSystem : public ex::System<ServerGameManagerSystem>, public ex::Receiver<ServerGameManagerSystem>
{
public:
	ServerGameManagerSystem();
	
	void update(ex::EntityManager& entities, ex::EventManager& events, ex::TimeDelta dt) override;

	/// Configures the system to subscribe events or getting managers.
	void configure(ex::EntityManager& entities, ex::EventManager& events) override;

	/// AddPlayerEvent event receive. Add player or bot to game. ClientIndex 0 is always game owner.
	void receive(const AddPlayerEvent& addPlayerEvent);

	/// StartGameEvent event receive. Creates ball to start the game.
	void receive(const StartGameEvent& startGameEvent);

	/// SendGameStateEvent event receive. Collects the game state and sends it to both clients.
	/// This function is called by server so that it works every server frame rate.
	void receive(const SendGameStateEvent& sendGameStateEvent);
	
	/// PauseEvent receiver. Toggles pause text and notifies clients to pause or resume the game.
	void receive(const PauseEvent& pauseEvent);

	/// RestartEvent receiver.
	void receive(const RestartEvent& restartEvent);

private:
	/// Resets all entities to their original position, and in the next state manager will send restarted state.
	void Restart() const;

	// Links of maanagers.
	entityx::EntityManager* entities;
	entityx::EventManager* events;
	// Game state
	uint64_t stateId;
};


#endif

