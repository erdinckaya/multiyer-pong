#ifdef EDITOR

#include "Editor.h"
#include "../World/Components/Destroy.h"


Editor Editor::Instance;

/// Renders reflection object. In order to use that feature, you need to
/// subscribe your component into editor eg: Translation.h
/// If you want to monitor your custom data, you can add it by following this function.
/// Although coding style is not very efficient, it is very easy to add your custom data.
/// Like I mentioned before this editor is created for debug purpose not release purpose.
void Editor::RenderObject(int& imgId, const ReflectObject& object) const
{
	auto oStr = object.name + "##" + std::to_string(imgId++);

	if (ImGui::TreeNode(oStr.c_str()))
	{
		for (auto& field : object.fields)
		{
			auto type = field.type;
			auto id = field.name + "##" + std::to_string(imgId++);
			if (type == "float")
			{
				ImGui::PushItemWidth(100);
				ImGui::DragFloat(id.c_str(), static_cast<float *>(field.ref), 0.02f, -FLT_MAX, FLT_MAX,
				                 "%.2f", 2.0f);
			}
			else if (type == "int")
			{
				ImGui::PushItemWidth(100);
				ImGui::DragInt(id.c_str(), static_cast<int *>(field.ref), 0.02f, INT_MIN, INT_MAX,
				               "%.2f");
			}
			else if (type == "float-rgb")
			{
				ImGui::PushItemWidth(100);
				ImGui::DragFloat(id.c_str(), static_cast<float *>(field.ref), 0.02f, 0.f, 1.f);
			}
			else if (type == "double")
			{
				ImGui::PushItemWidth(100);
				ImGui::InputDouble(id.c_str(), static_cast<double *>(field.ref));
			}
			else if (type == "long")
			{
				ImGui::PushItemWidth(100);
				auto l = (long *)(field.ref);
				auto str = std::to_string(*l);
				ImGui::InputText(id.c_str(), (char *)str.c_str(), str.capacity() + 1,
				                 ImGuiInputTextFlags_CallbackAlways, InputLongCallback, (void *)l);
			}
			else if (type == "string")
			{
				ImGui::PushItemWidth(100);
				auto str = (std::string *)(field.ref);
				ImGui::InputText(id.c_str(), (char *)str->c_str(), str->capacity() + 1,
				                 ImGuiInputTextFlags_CallbackResize, InputTextCallback, (void *)str);
			}
			else if (type == "bool")
			{
				ImGui::PushItemWidth(100);
				ImGui::Checkbox(id.c_str(), static_cast<bool*>(field.ref));
			}
			else if (type == "struct")
			{
				auto obj = static_cast<IReflect*>(field.ref)->Reflect();
				RenderObject(imgId, obj);
			}
			else if (type == "int2")
			{
				if (ImGui::TreeNode(id.c_str()))
				{
					auto val = static_cast<linalg::aliases::int2 *>(field.ref);

					id = "x##" + std::to_string(imgId++);
					ImGui::PushItemWidth(100);
					ImGui::DragInt(id.c_str(), &val->x, 0.02f, INT_MIN, INT_MAX,
					               "%.2f");

					id = "y##" + std::to_string(imgId++);
					ImGui::PushItemWidth(100);
					ImGui::DragInt(id.c_str(), &val->y, 0.02f, INT_MIN, INT_MAX,
					               "%.2f");

					ImGui::TreePop();
				}
			}
			else if (type == "int3")
			{
				if (ImGui::TreeNode(id.c_str()))
				{
					auto val = static_cast<linalg::aliases::int3 *>(field.ref);

					id = "x##" + std::to_string(imgId++);
					ImGui::PushItemWidth(100);
					ImGui::DragInt(id.c_str(), &val->x, 0.02f, INT_MIN, INT_MAX,
					               "%.2f");

					id = "y##" + std::to_string(imgId++);
					ImGui::PushItemWidth(100);
					ImGui::DragInt(id.c_str(), &val->y, 0.02f, INT_MIN, INT_MAX,
					               "%.2f");

					id = "z##" + std::to_string(imgId++);
					ImGui::PushItemWidth(100);
					ImGui::DragInt(id.c_str(), &val->z, 0.02f, INT_MIN, INT_MAX,
					               "%.2f");

					ImGui::TreePop();
				}
			}
			else if (type == "int4")
			{
				if (ImGui::TreeNode(id.c_str()))
				{
					auto val = static_cast<linalg::aliases::int4 *>(field.ref);

					id = "x##" + std::to_string(imgId++);
					ImGui::PushItemWidth(100);
					ImGui::DragInt(id.c_str(), &val->x, 0.02f, INT_MIN, INT_MAX,
					               "%.2f");

					id = "y##" + std::to_string(imgId++);
					ImGui::PushItemWidth(100);
					ImGui::DragInt(id.c_str(), &val->y, 0.02f, INT_MIN, INT_MAX,
					               "%.2f");

					id = "z##" + std::to_string(imgId++);
					ImGui::PushItemWidth(100);
					ImGui::DragInt(id.c_str(), &val->z, 0.02f, INT_MIN, INT_MAX,
					               "%.2f");

					id = "w##" + std::to_string(imgId++);
					ImGui::PushItemWidth(100);
					ImGui::DragInt(id.c_str(), &val->w, 0.02f, INT_MIN, INT_MAX,
					               "%.2f");

					ImGui::TreePop();
				}
			}
			else if (type == "float2")
			{
				if (ImGui::TreeNode(id.c_str()))
				{
					auto val = static_cast<linalg::aliases::float2 *>(field.ref);

					id = "x##" + std::to_string(imgId++);
					ImGui::PushItemWidth(100);
					ImGui::DragFloat(id.c_str(), &val->x, 0.02f, -FLT_MAX, FLT_MAX,
					                 "%.2f", 2.0f);

					id = "y##" + std::to_string(imgId++);
					ImGui::PushItemWidth(100);
					ImGui::DragFloat(id.c_str(), &val->y, 0.02f, -FLT_MAX, FLT_MAX,
					                 "%.2f", 2.0f);

					ImGui::TreePop();
				}
			}
			else if (type == "float3")
			{
				if (ImGui::TreeNode(id.c_str()))
				{
					auto val = static_cast<linalg::aliases::float3 *>(field.ref);

					id = "x##" + std::to_string(imgId++);
					ImGui::PushItemWidth(100);
					ImGui::DragFloat(id.c_str(), &val->x, 0.02f, -FLT_MAX, FLT_MAX,
					                 "%.2f", 2.0f);

					id = "y##" + std::to_string(imgId++);
					ImGui::PushItemWidth(100);
					ImGui::DragFloat(id.c_str(), &val->y, 0.02f, -FLT_MAX, FLT_MAX,
					                 "%.2f", 2.0f);

					id = "z##" + std::to_string(imgId++);
					ImGui::PushItemWidth(100);
					ImGui::DragFloat(id.c_str(), &val->z, 0.02f, -FLT_MAX, FLT_MAX,
					                 "%.2f", 2.0f);

					ImGui::TreePop();
				}
			}
			else if (type == "float4")
			{
				if (ImGui::TreeNode(id.c_str()))
				{
					auto val = static_cast<linalg::aliases::float4 *>(field.ref);

					id = "x##" + std::to_string(imgId++);
					ImGui::PushItemWidth(100);
					ImGui::DragFloat(id.c_str(), &val->x, 0.02f, -FLT_MAX, FLT_MAX,
					                 "%.2f", 2.0f);

					id = "y##" + std::to_string(imgId++);
					ImGui::PushItemWidth(100);
					ImGui::DragFloat(id.c_str(), &val->y, 0.02f, -FLT_MAX, FLT_MAX,
					                 "%.2f", 2.0f);

					id = "z##" + std::to_string(imgId++);
					ImGui::PushItemWidth(100);
					ImGui::DragFloat(id.c_str(), &val->z, 0.02f, -FLT_MAX, FLT_MAX,
					                 "%.2f", 2.0f);

					id = "w##" + std::to_string(imgId++);
					ImGui::PushItemWidth(100);
					ImGui::DragFloat(id.c_str(), &val->w, 0.02f, -FLT_MAX, FLT_MAX,
					                 "%.2f", 2.0f);

					ImGui::TreePop();
				}
			}
			else if (type == "sprite_type")
			{
				auto str = std::string(static_cast<SpriteType*>(field.ref)->_to_string());
				ImGui::InputText(id.c_str(), (char*)str.c_str(), str.capacity() + 1, ImGuiInputTextFlags_ReadOnly);
			}
			else if (type == "collider_type")
			{
				auto str = std::string(static_cast<ColliderType*>(field.ref)->_to_string());
				ImGui::InputText(id.c_str(), (char*)str.c_str(), str.capacity() + 1, ImGuiInputTextFlags_ReadOnly);
			}
			else if (type == "font_type")
			{
				auto str = std::string(static_cast<FontType*>(field.ref)->_to_string());
				ImGui::InputText(id.c_str(), (char*)str.c_str(), str.capacity() + 1, ImGuiInputTextFlags_ReadOnly);
			}
			else if (type == "network_object_type")
			{
				auto str = std::string(static_cast<NetworkObjectType*>(field.ref)->_to_string());
				ImGui::InputText(id.c_str(), (char*)str.c_str(), str.capacity() + 1, ImGuiInputTextFlags_ReadOnly);
			}
			else if (type == "uint64_t")
			{
				auto str = std::to_string(*static_cast<uint64_t*>(field.ref));
				ImGui::InputText(id.c_str(), (char*)str.c_str(), str.capacity() + 1, ImGuiInputTextFlags_ReadOnly);
			}
		}

		ImGui::TreePop();
	}
}

/// Main render function.
void Editor::Render() const
{
	bool show = true;
	ImGui::Begin("Editor", &show);

	const ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	int imgId = 0;


	for (const auto& group : groups)
	{
		auto gStr = group.name + "##" + std::to_string(imgId++);
		if (ImGui::TreeNode(gStr.c_str()))
		{
			for (const auto& object : group.objects)
			{
				RenderObject(imgId, object);
			}

			ImGui::TreePop();
		}
	}

	for (const auto& group : groups)
	{
	}
	ImGui::End();
}

/// Text field editor function
int Editor::InputTextCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		// Resize string callback
		auto str = (std::string *)data->UserData;
		IM_ASSERT(data->Buf == str->c_str());
		str->resize(static_cast<unsigned long>(data->BufTextLen));
		data->Buf = (char *)str->c_str();
	}
	return 0;
}

/// Long field editor function
int Editor::InputLongCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackAlways)
	{
		const auto ptr = static_cast<long*>(data->UserData);
		try
		{
			*ptr = std::stol(data->Buf);
		}
		catch (const std::logic_error)
		{
		}
	}
	return 0;
}


#if (defined SERVER && defined EDITOR)

Editor::Editor() : sdl(std::make_unique<SDL>(SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL))
{
}
#endif

int Editor::StartEditor()
{
	spdlog::info("Editor has started!");
	auto flags = static_cast<SDL_WindowFlags>(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
#ifdef SERVER
	const char* title = "Server EDITOR";
#else
	const char* title = "EDITOR";
#endif
	window = SDL_CreateWindow(title, 0, 50, 800, 800, flags);

	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL2_Init();

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// Main loop
	bool done = false;
	while (!done)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				done = true;
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		FetchData();
		Render();

		// Rendering
		ImGui::Render();
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
	}

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

/// Fetches all entities and components from world.
/// Note that, it is very slow process.
/// In order to monitor your component you need to add it there.
void Editor::FetchData()
{
	std::vector<ReflectGroup> groups;
	EntityManager* entities = &World::Instance.entities;
	for (auto entity : entities->entities_for_debugging())
	{
		if (!entity.valid() || entity.has_component<Destroy>())
		{
			continue;
		}
		
		ReflectGroup group;
		group.name = std::to_string(entity.id().id());
		if (entity.has_component<Name>())
		{
			group.name = entity.component<Name>()->value;
		}
		// Subscribe your component.
		RenderComponent<RenderTarget, Translation, Scale, Size, RenderOrder,
		                RectangleShape, CircleShape, Image,
		                Move, Player, Rival, Ball, Text, NetworkObject, PlayerNetworkInfo,
						Bot,
		                EndOfList>(entity, group);
		groups.push_back(group);
	}
	Instance.groups = groups;
}

/// Sets window title.
void Editor::SetWindowTitle(const char* title)
{
	spdlog::debug("Setting editor title as {0} window null check {1}", title, window == nullptr);
	SDL_SetWindowTitle(window, title);
}
#endif