#pragma once
#include "../Editor/IReflect.h"

/// Color util with RGBA
struct Color ADD_REFLECTION
{
	Color(const float& r = 1.f, const float& g = 1.f, const float& b = 1.f,
	               const float& a = 1.f) : r(r), g(g), b(b), a(a)
	{
	}

	float r;
	float g;
	float b;
	float a;

	START_REFLECTION(Color)
		ADD_REFLECTION_FIELD(r, "float-rgb")
		ADD_REFLECTION_FIELD(g, "float-rgb")
		ADD_REFLECTION_FIELD(b, "float-rgb")
		ADD_REFLECTION_FIELD(a, "float-rgb")
		END_REFLECTION
};
