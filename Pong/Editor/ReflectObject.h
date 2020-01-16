#pragma once
#include <string>
#include <vector>
#include "ReflectField.h"

/// Keeps whole objects reflection data
struct ReflectObject
{
	std::string name;
	std::vector<ReflectField> fields;
};
