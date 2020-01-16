#pragma once
#include <utility>
#include "../../Editor/IReflect.h"
#include "../../Util/Color.h"

// Tag Component for circle shapes
struct CircleShape ADD_REFLECTION
{
	explicit CircleShape(const float& radius = 30.f, Color color = {0, 0, 1, 1}, const bool& fill = false,
	                     const int& lineWidth = 3, const bool render = false) : color(std::move(color)), fill(fill),
	                                                                            render(render), lineWidth(lineWidth),
	                                                                            radius(radius)
	{
	}

	Color color;
	bool fill;
	bool render;
	int lineWidth;
	float radius;

	START_REFLECTION(CircleShape)
		ADD_REFLECTION_OBJECT(color)
		ADD_REFLECTION_FIELD(fill, "bool")
		ADD_REFLECTION_FIELD(lineWidth, "int")
		ADD_REFLECTION_FIELD(radius, "float")
		ADD_REFLECTION_FIELD(render, "bool")
		END_REFLECTION
};
