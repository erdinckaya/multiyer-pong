#pragma once
#include <string>

/// Keeps fields' reflection data
struct ReflectField
{
	std::string name;
	std::string type;
	void* ref;

	ReflectField(const char* name, const char* type, void* ref) : name(name), type(type), ref(ref)
	{
		if (this->type == "string")
		{
			auto str = static_cast<std::string*>(ref);
		}
	}
};
