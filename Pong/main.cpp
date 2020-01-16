
#include <GL/glew.h>
#include "Graphics/Graphics.h"
#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>
#include "Network/NetworkManager.h"
#include "Graphics/GraphicsManager.h"
#include "World/World.h"



#ifdef EDITOR
#include "Editor/editor.h"
#endif



int main(int argc, char** argv)
{
	// Setting spdlog pattern and level.
	spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [th %t] %v");
	spdlog::set_level(spdlog::level::debug);

	// Starting graphics
	GraphicsManager::Instance.Init();


	// Start Editor
#ifdef EDITOR
	auto editorThread = std::make_unique<std::thread>([&](Editor* e) { e->StartEditor(); }, &Editor::Instance);
#endif
	SDL_Delay(100);
	// Start networking and Yojimbo.
	auto networkThread = std::make_unique<std::thread>([&](NetworkManager* n)
	{
		n->network->Run();
		// Free itself after networking done, which means error or user closed the game.
		NetworkManager::Instance.CommitSuicide();
	}, &NetworkManager::Instance);
	

	SDL_Delay(100);
	// World has created and started to spin to give life to all things.
	// Main function for game.
	World::Instance.Spin();
	// Stop networking to prepare itself to close the application.
	NetworkManager::Instance.network->Stop(); 

	// Detach thread to wait Yojimbo to close.
	networkThread->detach();
	SDL_Delay(100);

	return 0;
}
