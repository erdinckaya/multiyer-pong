#pragma once

#include <entityx/entityx.h>
#include <unordered_set>


struct RemoveTextEvent;
struct ChangeTextEvent;
namespace ex = entityx;

class TextSystem : public ex::System<TextSystem>, public ex::Receiver<TextSystem>
{
public:
	/// Configures the system to subscribe events or getting managers.
	void configure(entityx::EventManager& events) override;

	/// Updates texts values and notifies the graphics to render them.
	/// Every text must have unique textId. See `TextId`
	void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;

	/// ChangeTextEvent receiver. Adds texts to change buffer.
	void receive(const ChangeTextEvent& changeTextEvent);

	/// RemoveTextEvent receiver. Adds texts to remove buffer.
	void receive(const RemoveTextEvent& removeTextEvent);

private:
	// Remove buffer.
	std::unordered_set<int> removeEvents;
	// Change buffer.
	std::unordered_map<int, std::string> changeEvents;
};
