#pragma once
#include <utility>
#include "../../Editor/IReflect.h"
#include "../../Util/Color.h"

/// Keeps info of shape
struct RectangleShape ADD_REFLECTION
{
	explicit RectangleShape(Color color = {0, 0, 1, 1}, const bool& fill = false,
	                        const int& lineWidth = 3, const bool& render = false) : color(std::move(color)), fill(fill),
	                                                                                lineWidth(lineWidth), render(render)
	{
	}

	Color color;
	bool fill;
	int lineWidth;
	bool render;

	START_REFLECTION(RectangleShape)
		ADD_REFLECTION_OBJECT(color)
		ADD_REFLECTION_FIELD(fill, "bool")
		ADD_REFLECTION_FIELD(lineWidth, "int")
		ADD_REFLECTION_FIELD(render, "bool")
		END_REFLECTION
};
