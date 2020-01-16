#pragma once
#include <yojimbo.h>

struct PauseEvent
{
	PauseEvent(yojimbo::Message* message = nullptr) : message(message) {}

	yojimbo::Message* message;
};
