#include "Graphics.h"
#include <spdlog/spdlog.h>

// Remove DrawText Window's macro 
#ifdef DrawText
#undef DrawText
#endif

/// Initializes graphics with opengl and asset manager as well.
Graphics::Graphics(int width, int height, const char* title, int flags)
	: sdl(flags), window(width, height, title, flags), context(window), width(width), height(height)
{
	glewInit();


	glEnable(GL_TEXTURE_2D);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glClear(GL_COLOR_BUFFER_BIT);
	window.Show();

	AssetManager::Instance.LoadAssets();

	LoadFonts();
}

/// Release all fonts.
Graphics::~Graphics()
{
	// Free Glyphs
	for (int i = 0; i < FontType::Count; ++i)
	{
		for (int j = 1; j < FONT_SIZE_MAX; ++j)
		{
			for (auto glyph : fonts[i][j].glyphs)
			{
				glDeleteTextures(1, &glyph.second.textureId);
			}
		}
	}
}

/// Updates all screen, redraws it.
void Graphics::Update(float dt) const
{
	SDL_GL_MakeCurrent(window, context);
	SDL_GL_SwapWindow(window);
}

/// Draws rectangle with given parameters.
void Graphics::DrawRect(const float4& rect, const Color& color, const int& lineWidth, const bool& fill)
{
	glColor4f(color.r, color.g, color.b, color.a);
	glLineWidth(lineWidth);

	if (fill)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	glBegin(GL_POLYGON);

	glVertex2f(rect.x, rect.y);
	glVertex2f(rect.x + rect.w, rect.y);
	glVertex2f(rect.x + rect.w, rect.y + rect.z);
	glVertex2f(rect.x, rect.y + rect.z);
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glFlush();
}

/// Draws circle with given parameters.
void Graphics::DrawCircle(const float2 center, float radius, const Color& color, const int& lineWidth, const bool& fill)
{
	glColor4f(color.r, color.g, color.b, color.a);
	glLineWidth(lineWidth);

	static const int circle_points = 100;
	static const float angle = 2.0f * 3.1416f / circle_points;

	if (fill)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	// this code (mostly) copied from question:
	glBegin(GL_POLYGON);

	double angle1 = 0.0;
	//glVertex2d(radius * cos(0.0), radius * sin(0.0));
	for (int i = 0; i < circle_points; i++)
	{
		glVertex2d(radius * cos(angle1) + center.x, radius * sin(angle1) + center.y);
		angle1 += angle;
	}
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glFlush();
}

/// Changes orthoganal camera position.
void Graphics::SetCameraPos(const int2& pos)
{
	glViewport(static_cast<GLint>(pos.x), static_cast<GLint>(pos.y), width, height);
}

/// Changes camera's view port.
void Graphics::SetCameraViewPort(const int4& port)
{
	width = static_cast<unsigned int>(port.w);
	height = static_cast<unsigned int>(port.z);
	SetCameraPos(int2(port.x, port.y));
}

/// Draws an image into opengl context.
void Graphics::DrawImage(SDLSurface* s, float4 destination, float rotationZ, bool flipY, bool flipX)
{
	glEnable(GL_TEXTURE_2D);

	s->Bind();


	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex2f(destination.x, destination.y + destination.z);
	glTexCoord2f(1, 1);
	glVertex2f(destination.x + destination.w, destination.y + destination.z);
	glTexCoord2f(1, 0);
	glVertex2f(destination.x + destination.w, destination.y);
	glTexCoord2f(0, 0);
	glVertex2f(destination.x, destination.y);

	glEnd();
	// draw model
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}


/// Draws texts with given parameters.
void Graphics::DrawText(const char* text, FontType fontType, int fontSize, const Color& color, float2 pos, float scale)
{
	const auto font = fonts[fontType][fontSize];

	auto shader = AssetManager::Instance[ShaderType::Text];
	// Activate corresponding render state
	shader.Use();
	glm::mat4 projection = glm::ortho(-(int)width * 0.5f, (int)width * 0.5f, -(int)height * 0.5f, (int)height * 0.5f);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.r, color.g, color.b);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);



	int x = pos.x;
	int y = pos.y - fontSize * scale;
	float firstX = x;
	float line = 0;
	// Iterate through all characters
	for (; *text; ++text)
	{
		if (*text == '\n' || *text == '\r')
		{
			line++;
			x = firstX;
			continue;
		}
		const Glyph ch = GetGlyph(text, fontType, fontSize);



		const GLfloat xpos = x + ch.bearing.x * scale;
		const GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale - line * font.size * scale;

		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{xpos, ypos + h, 0.0, 0.0},
			{xpos, ypos, 0.0, 1.0},
			{xpos + w, ypos, 1.0, 1.0},

			{xpos, ypos + h, 0.0, 0.0},
			{xpos + w, ypos, 1.0, 1.0},
			{xpos + w, ypos + h, 1.0, 0.0}
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.textureId);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER,
		                0,
		                sizeof(vertices),
		                vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		// Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
		x += (ch.advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

/// Calculates text lengths.
float Graphics::CalculateTextWidth(const char* text, const FontType& fontType, const int& fontSize, const float& scale)
{
	float advance = 0;
	for (; *text; ++text)
	{
		Glyph g{};
		if (fonts[fontType][fontSize].glyphs.find(*text) == fonts[fontType][fontSize].glyphs.end())
		{
			g = fonts[fontType][fontSize].glyphs['_'];
		}
		else
		{
			g = fonts[fontType][fontSize].glyphs[*text];
		}

		advance += g.advance >> 6;
	}
	return advance * scale;
}

/// Calculates text heigth actually it is font size with scale 1.0f.
float Graphics::CalculateTextHeight(const char* text, const FontType& fontType, const int& fontSize, const float& scale)
{
	return fontSize * scale;
}

/// Returns text width and height.
float2 Graphics::CalculateTextSize(const char* text, const FontType& fontType, const int& fontSize, const float& scale)
{
	return float2(CalculateTextWidth(text, fontType, fontSize, scale), fontSize);
}

/// Changes window's title.
void Graphics::SetWindowTitle(const char* title)
{
	window.SetWindowTitle(title);
}

/// Changes windows direction in your monitor.
/// If you have two monitors it moves window to second monitor.
void Graphics::SetWindowDirection(const Direction& direction)
{
	window.SetWindowDirection(direction);
}

/// Loads fonts.
void Graphics::LoadFonts()
{
	// First get font infoes.
	for (int j = 0; j < FontType::Count; ++j)
	{
		const auto asset = FontType::_from_integral(j);
		for (int i = 1; i < FONT_SIZE_MAX; ++i)
		{
			fonts[j][i].size = i;
			fonts[j][i].type = FontType::_from_integral(j);
			fonts[j][i].filePath = AssetManager::Instance[asset].c_str();
		}
	}


	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
	{
		spdlog::critical("ERROR::FREETYPE: Could not init FreeType Library");
	}


	for (int i = 0; i < FontType::Count; ++i)
	{
		FT_Face face{};
		for (int j = 1; j < FONT_SIZE_MAX; ++j)
		{
			// Load font as face
			if (FT_New_Face(ft, fonts[i][j].filePath, 0, &face))
			{
				spdlog::critical("ERROR::FREETYPE: Failed to load font");
			}

			// Set size to load glyphs as
			FT_Set_Pixel_Sizes(face, 0, fonts[i][j].size);

			// Disable byte-alignment restriction
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			for (GLubyte c = 0; c < 128; c++)
			{
				// Load character glyph
				if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				{
					spdlog::critical("ERROR::FREETYTPE: Failed to load Glyph");
					continue;
				}
				// Generate texture
				GLuint texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					face->glyph->bitmap.buffer
				);
				// Set texture options
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				// Now store character for later use
				Glyph glyph = {
					texture,
					glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					face->glyph->advance.x
				};
				fonts[i][j].glyphs[c] = glyph;
			}
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		FT_Done_Face(face);
	}
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

/// Fetch a single glyph from asset manager.
Glyph Graphics::GetGlyph(const char* text, FontType fontType, int fontSize)
{
	Glyph ch = {};
	if (fonts[fontType][fontSize].glyphs.find(*text) == fonts[fontType][fontSize].glyphs.end())
	{
		ch = fonts[fontType][fontSize].glyphs['_'];
	}
	else
	{
		ch = fonts[fontType][fontSize].glyphs[*text];
	}

	return ch;
}
