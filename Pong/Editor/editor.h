#pragma once

#ifdef EDITOR


#include <imgui.h>
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"
#include <cstdio>
#include <SDL2/SDL.h>
#include <vector>
#include "ReflectGroup.h"

#include "../Util/UtilMacro.h"
#include "../Util/Types.h"
#include <GL/glew.h>


#include "IReflect.h"
#include <iostream>
#include <linalg.h>
#include "../Graphics/SDL.h"
#include <entityx/Entity.h>



#include "../World/World.h"
#include "../World/Components/Name.h"

#include "../World/Components/RenderTarget.h"
#include "../World/Components/Translation.h"
#include "../World/Components/Scale.h"
#include "../World/Components/Size.h"
#include "../World/Components/RenderOrder.h"
#include "../World/Components/RectangleShape.h"
#include "../World/Components/CircleShape.h"
#include "../World/Components/Image.h"
#include "../World/Components/Move.h"
#include "../World/Components/Player.h"
#include "../World/Components/Rival.h"
#include "../World/Components/Ball.h"
#include "../World/Components/Text.h"
#include "../World/Components/NetworkObject.h"
#include "../World/Components/PlayerNetworkInfo.h"
#include "../World/Components/Bot.h"

struct EndOfList;

/// Registers components and tracks them all.
/// After all opens an window and lets developer to play with it.
/// In order to activate Editor you need to add EDITOR macro into
/// build configs, you can activate EDITOR both for server and client.
class Editor
{
	void RenderObject(int& imgId, const ReflectObject& object) const;
	void Render() const;
	static int InputTextCallback(ImGuiInputTextCallbackData* data);
	static int InputLongCallback(ImGuiInputTextCallbackData* data);

	template <typename C>
	void RenderComponent(entityx::Entity entity, ReflectGroup& group)
	{
		//static_assert(std::is_same<C, EndOfList>::value, "error");
	}

	template <typename C, typename C1, typename... Components>
	void RenderComponent(entityx::Entity entity, ReflectGroup& group)
	{
		if (entity.has_component<C>())
		{
			auto reflect = dynamic_cast<IReflect*>(entity.component<C>().get());
			if (reflect != nullptr)
			{
				group.objects.push_back(reflect->Reflect());
			}
		}

		RenderComponent<C1, Components...>(entity, group);
	}

	void FetchData();


	SDL_Window* window;


public:

	std::unique_ptr<SDL> sdl;
	static Editor Instance;

	std::vector<ReflectGroup> groups;
#if (defined SERVER && defined EDITOR)
	Editor();
#endif

	int StartEditor();


	void SetWindowTitle(const char* title);
};
#endif