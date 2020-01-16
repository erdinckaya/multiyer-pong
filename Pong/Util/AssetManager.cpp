#include "AssetManager.h"

#include <memory>
#include "../Graphics/SDLSurface.h"
#include "Types.h"

#include  <sstream>
#include "../Graphics/Shader.h"
#include "UtilMacro.h"

/// Singleton for AssetManager.
AssetManager AssetManager::Instance;

AssetManager::AssetManager()
{
}

/// Loads all assets
void AssetManager::LoadAssets()
{
	LoadSprites();
	LoadShaders();
	LoadFonts();
}

/// Loads all sprites that under the Resources folder.
void AssetManager::LoadSprites()
{
	std::stringstream stream;
	std::string path;
	for (int i = 0; i < SpriteType::Count; i++)
	{
		auto asset = SpriteType::_from_integral(i);
		stream << "." << separator() << "Resources" << separator() << asset._to_string() << ".png";
		path = stream.str();
		DEBUG_LOG("Loading asset {0}", path);
		sprites[asset._to_integral()] = std::make_unique<SDLSurface>(path.c_str());
		stream.str("");
	}
}

/// Loads all shaders that under the Resources folder.
void AssetManager::LoadShaders()
{
	std::stringstream stream;
	std::string vertex;
	std::string fragment;
	for (int i = 0; i < ShaderType::Count; i++)
	{
		auto asset = ShaderType::_from_integral(i);
		stream << "." << separator() << "Resources" << separator() <<asset._to_string() << ".vert";
		vertex = stream.str();
		stream.str("");

		stream << "." << separator() << "Resources" << separator() << asset._to_string() << ".frag";
		fragment = stream.str();
		stream.str("");
		
		DEBUG_LOG("Compiling shader {0} vertex: {1} fragment: {2}", asset._to_string(), vertex, fragment);
		shaders[asset._to_integral()].Compile(vertex.c_str(), fragment.c_str());
	}
}

/// Loads all fonts that under the Resources folder.
void AssetManager::LoadFonts()
{
	std::stringstream stream;
	std::string path;
	for (int j = 0; j < FontType::Count; ++j)
	{
		auto asset = FontType::_from_integral(j);
		stream << "." << separator() << "Resources" << separator() << asset._to_string() << ".ttf";
		fonts[j] = stream.str();
	}
}

/// Fetches sprites.
SDLSurface* AssetManager::operator[](SpriteType asset) const
{
	return sprites[asset].get();
}

/// Fetches shaders.
Shader& AssetManager::operator[](ShaderType type)
{
	return shaders[type];
}

/// Fetches fonts.
std::string& AssetManager::operator[](FontType type)
{
	return fonts[type];
}
