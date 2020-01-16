#pragma once
#include <vector>
#include "ReflectObject.h"

/// Keeps whole group of objects' reflection data
struct ReflectGroup
{
	std::string name;
	std::vector<ReflectObject> objects;
};
