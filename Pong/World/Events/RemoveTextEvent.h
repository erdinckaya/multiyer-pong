#pragma once


struct RemoveTextEvent
{
	explicit RemoveTextEvent(const int& id) : id(id)
	{
	}

	int id;
};
