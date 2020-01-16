#pragma once
#include <string>
#include <utility>


struct ChangeTextEvent
{
	explicit ChangeTextEvent(const int& id, std::string text) : id(id), text(std::move(text))
	{
	}

	int id;
	std::string text;
};
