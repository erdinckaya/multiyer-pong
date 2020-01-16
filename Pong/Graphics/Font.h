#pragma once
#include "../Util/Types.h"
#include <unordered_map>
#include "Glyph.h"

/// Simple font data struct.
struct Font
{
	Font(int size = 0, FontType type = FontType::Count, const char* filePath = nullptr) : size(size), type(type),
		filePath(filePath)
	{
	}

	int size;
	FontType type;
	const char* filePath;
	std::unordered_map<char, Glyph> glyphs;
};
