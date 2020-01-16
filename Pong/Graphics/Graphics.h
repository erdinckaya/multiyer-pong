#pragma once

#include "Shader.h"

#include <unordered_map>
#include "SDLWindow.h"
#include "SDLContext.h"
#include "SDL.h"
#include "SDLSurface.h"


#include <linalg.h>
#include "../Util/Color.h"
#include "../Util/Constants.h"
#include "Font.h"


#include "../Util/AssetManager.h"
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H



using namespace linalg::aliases;

/// This is the main class that handles all kind of graphic processes.
/// If you want to change your graphic backend you need to change only this
/// class at all.
class Graphics
{
public:

	Graphics(int width, int height, const char* title, int flags);

	~Graphics();

	void DrawRect(const float4& rect, const Color& color, const int& lineWidth, const bool& fill);

	void DrawCircle(const float2 center, float radius, const Color& color, const int& lineWidth, const bool& fill);
	
	void Update(float dt) const;

	void SetCameraPos(const int2& pos);

	void SetCameraViewPort(const int4& port);

	void DrawImage(SDLSurface* surface, float4 destination, float rotationZ, bool flipY, bool flipX);

	void DrawText(const char* text, FontType fontType, int fontSize, const Color& color, float2 pos, float scale);

	float CalculateTextWidth(const char* text, const FontType& fontType, const int& fontSize, const float& scale = 1.f);
	float CalculateTextHeight(const char* text, const FontType& fontType, const int& fontSize, const float& scale = 1.f);
	float2 CalculateTextSize(const char* text, const FontType& fontType, const int& fontSize, const float& scale = 1.f);

	void SetWindowTitle(const char* title);

	void SetWindowDirection(const Direction& direction);

private:

	void LoadFonts();

	Glyph GetGlyph(const char* text, FontType fontType, int fontSize);
	
	SDL sdl;
	SDLWindow window;
	SDLContext context;
	unsigned int width;
	unsigned int height;

	Font fonts[FontType::Count][FONT_SIZE_MAX];

	GLuint VAO, VBO;
};
