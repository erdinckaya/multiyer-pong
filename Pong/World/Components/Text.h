#pragma once

#include <utility>
#include "../../Editor/IReflect.h"
#include "../../Util/Types.h"
#include "../../Util/Color.h"

/// Keeps text info
struct Text ADD_REFLECTION
{
	explicit Text(const int& id, std::string value, const FontType& fontType = FontType::Fontana, const int& fontSize = 25,
	              Color color = {1, 1, 1, 1})
		: value(std::move(value)), id(id), fontType(fontType), fontSize(fontSize), color(std::move(color))
	{
	}

	std::string value;
	int id;
	FontType fontType;
	int fontSize;
	Color color;


	START_REFLECTION(Text)
		ADD_REFLECTION_FIELD(value, "string")
		ADD_REFLECTION_FIELD(id, "int")
		ADD_REFLECTION_FIELD(fontType, "font_type")
		ADD_REFLECTION_FIELD(fontSize, "int")
		ADD_REFLECTION_OBJECT(color)
		END_REFLECTION
};
