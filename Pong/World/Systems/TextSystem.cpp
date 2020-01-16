#include "TextSystem.h"

#include "../../Graphics/GraphicsManager.h"


#include "../Components/Destroy.h"
#include "../Components/Scale.h"
#include "../Components/Translation.h"
#include "../Components/Text.h"

#include "../Events/RemoveTextEvent.h"
#include "../Events/ChangeTextEvent.h"


void TextSystem::configure(entityx::EventManager& events)
{
	events.subscribe<ChangeTextEvent>(*this);
	events.subscribe<RemoveTextEvent>(*this);
}


void TextSystem::update(ex::EntityManager& entities, ex::EventManager& events, ex::TimeDelta dt)
{
	// Check graphics is ready or not
	if (!GraphicsManager::Instance.Ready())
	{
		return;
	}

	// Get graphics.
	auto graphics = GraphicsManager::Instance.GetGraphics();

	// Traverse all text entities.
	entities.each<Text, Translation, Scale>([this, graphics](ex::Entity entity, Text& text, Translation& translation, Scale& scale)
	{
		// If text is marked for remove, then remove.
		if (removeEvents.find(text.id) != removeEvents.end())
		{
			entity.assign<Destroy>();
		}

		// If text is changed, change textes.
		if (changeEvents.find(text.id) != changeEvents.end())
		{
			text.value = std::move(changeEvents[text.id]);
		}

		// Render them.
		graphics->DrawText(text.value.c_str(), text.fontType, text.fontSize, text.color, translation.value, scale.value.x);
	});

	// Clear buffers.
	removeEvents.clear();
	changeEvents.clear();
}


void TextSystem::receive(const ChangeTextEvent& changeTextEvent)
{
	if (changeEvents.find(changeTextEvent.id) == changeEvents.end())
	{
		changeEvents[changeTextEvent.id] = changeTextEvent.text;
	}
}


void TextSystem::receive(const RemoveTextEvent& removeTextEvent)
{
	if (removeEvents.find(removeTextEvent.id) == removeEvents.end())
	{
		removeEvents.insert(removeTextEvent.id);
	}}