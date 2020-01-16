#pragma once

/// Simple glyph data struct.
struct Glyph
{
	GLuint textureId; // ID handle of the glyph texture
	glm::ivec2 size; // Size of glyph
	glm::ivec2 bearing; // Offset from baseline to left/top of glyph
	signed long advance; // Horizontal offset to advance to next glyph
};
