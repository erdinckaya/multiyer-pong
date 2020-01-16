#pragma once

#include <entityx/entityx.h>
#include <linalg.h>


struct Move;
struct Translation;
struct Size;
struct UpdatePlayerMoveEvent;

namespace ex = entityx;
using namespace linalg::aliases;

class PlayerMoveSystem : public ex::System<PlayerMoveSystem>, public ex::Receiver<PlayerMoveSystem>
{
public:
	/// This system is responsible from both rival and player's moves.
	PlayerMoveSystem();

	/// Configure system and subscribe to UpdatePlayerMoveEvent.
	void configure(entityx::EntityManager& entities, entityx::EventManager& events) override;

	/// Updates paddles moves, handles user inputs as well.
	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;

	/// Advances player's movement respect to its direction speed and time.
	static void Integrate(Translation& translation, Move& move, const Size& size, const float& dt);
	
	/// UpdatePlayerMoveEvent receiver. This functions handles server corrections and only works for CLIENT.
	void receive(const UpdatePlayerMoveEvent& event);
private:
#ifdef CLIENT
	/// Handles user inputs.
	static void HandleInputs(Move& move);

	/// Creates a message and sends it to server.
	static void SendPlayerMoveMessage(const float2& direction, const uint64_t& clientId);
#endif
	entityx::EntityManager* entities;
};
