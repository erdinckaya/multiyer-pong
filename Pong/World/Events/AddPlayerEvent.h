#pragma once
#include <entityx/Entity.h>


struct AddPlayerEvent
{
	explicit AddPlayerEvent(const entityx::Entity& entity, const bool& owner, const int& clientIndex,
	                        const uint64_t& clientId) : owner(owner), clientIndex(clientIndex), clientId(clientId),
	                                                    entity(entity)
	{
	}


	bool owner;
	int clientIndex;
	uint64_t clientId;
	entityx::Entity entity;
};
