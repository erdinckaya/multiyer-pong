#pragma once
#include <memory>
#include "../Graphics/SDLSurface.h"
#include "Types.h"
#include "../Graphics/Shader.h"

/// File searator.
inline char separator()
{
#ifdef _WIN32
	return '\\';
#else
	return '/';
#endif
}

/// AssetManager is responsible from loading and storing assets.
/// such as images and shaders.
class AssetManager
{
public:
	static AssetManager Instance;

	AssetManager();

	void LoadAssets();

	SDLSurface* operator[](SpriteType asset) const;

	Shader& operator[](ShaderType type);
	
	std::string& operator[](FontType type);

private:

	void LoadSprites();

	void LoadShaders();
	
	void LoadFonts();

	std::unique_ptr<SDLSurface> sprites[SpriteType::Count];

	Shader shaders[ShaderType::Count];
	std::string fonts[FontType::Count];
};
