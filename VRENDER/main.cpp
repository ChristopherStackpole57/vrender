#include <iostream>

#include "Configuration/VulkanConfig.h"

#include <vulkan/vulkan.h>

#include <ame/AME.h>

#include <PlatformLayer/Utility/Event.h>
#include <PlatformLayer/Utility/WindowMode.h>
#include <PlatformLayer/WindowBackends/GLFWWindowBackend.h>

#include <RenderLayer/Core/Renderer.h>
#include <RenderLayer/Core/Mesh.h>
#include <RenderLayer/Core/GeometryArena.h>

#include <RenderLayer/Configuration/InstanceConfiguration.h>

#include <EngineLayer/Core/RenderService.h>
#include <EngineLayer/Core/RuntimeScheduler.h>
#include <EngineLayer/Core/SceneService.h>
#include <EngineLayer/Core/ServiceManager.h>

#ifdef NDEBUG
const bool ENABLE_VALIDATION_LAYERS = false;
#else
const bool ENABLE_VALIDATION_LAYERS = true;
#endif

void create_game_scene1()
{
	vrender::engine::SceneService* scene_service = vrender::engine::Services().Get<vrender::engine::SceneService>();

	// Floor
	using namespace vrender::engine;

	InstanceHandle floor = scene_service->CreateInstanceOfMesh("cube");
	Transform& floor_transform = scene_service->GetInstanceTransform(floor);
	floor_transform.position = ame::vec3f{ 0.0f, -1.5f, -5.0f };
	floor_transform.scale = ame::vec3f{ 3.0f, 1.0f, 5.0f };

	InstanceHandle left_wall = scene_service->CreateInstanceOfMesh("cube");
	Transform& left_wall_transform = scene_service->GetInstanceTransform(left_wall);
	left_wall_transform.position = ame::vec3f{ -2.0f, 1.0f, -5.0f };
	left_wall_transform.scale = ame::vec3f{ 1.0f, 4.0f, 5.0f };

	InstanceHandle right_wall = scene_service->CreateInstanceOfMesh("cube");
	Transform& right_wall_transform = scene_service->GetInstanceTransform(right_wall);
	right_wall_transform.position = ame::vec3f{ 2.0f, 1.0f, -5.0f };
	right_wall_transform.scale = ame::vec3f{ 1.0f, 4.0f, 5.0f };
}

/*
static_assert(sizeof(ame::mat4f) == 64);
static_assert(alignof(ame::mat4f) == 16);
int main()
{
	/*
	Create a shared pointer to a window provider, and from it generate a pointer to a surface provider.
	Because all window backends inherit from both the WindowProvider and WindowSurfaceProvider this pointer conversion is valid.
	Holding the pointer as window provider allows access to the WindowProvider API which will be interacted with in the primary
	execution loop. The surface provider, on the other hand, is necessary for generating a VkSurfaceKHR in the Instance constructor.
	*/
	/*

	// Platform Setup
	std::shared_ptr<vrender::platform::WindowProvider> window_provider_ptr = std::make_shared<vrender::platform::GLFWWindowBackend>();
	std::shared_ptr<vrender::platform::WindowSurfaceProvider> surface_provider_ptr = std::dynamic_pointer_cast<
		vrender::platform::WindowSurfaceProvider
	>(window_provider_ptr);

	vrender::render::config::InstanceConfig instance_config{
		.enable_validation = ENABLE_VALIDATION_LAYERS,
	};
	//instance_config.extensions = std::vector<std::string>{};					// Not needed yet

	// Configure Window
	window_provider_ptr->set_title("VRENDER Engine");
	window_provider_ptr->set_resizable(true);





	// Engine Setup
	vrender::engine::RuntimeScheduler runtime_scheduler;

	vrender::engine::Services().RegisterService<vrender::engine::RenderService>(
		*window_provider_ptr,
		*surface_provider_ptr,
		instance_config
	);
	vrender::engine::RenderService* render_service = vrender::engine::Services().Get<vrender::engine::RenderService>();
	vrender::engine::Services().RegisterService<vrender::engine::SceneService>(
		render_service
	);
	vrender::engine::SceneService* scene_service = vrender::engine::Services().Get<vrender::engine::SceneService>();

	runtime_scheduler.SetServiceTickPriority(
		render_service,
		vrender::engine::SCHEDULE_TICK_LEVEL_FRAMERENDER
	);
	runtime_scheduler.SetServiceTickPriority(
		scene_service,
		vrender::engine::SCHEDULE_TICK_LEVEL_FRAMEPREP
	);





	// Asset Setup
	// Bind Cube
	std::vector<vrender::render::Vertex> cube_vertices = {
		{{ -0.5f, -0.5f,  0.5f }, {0, 1, 0}},
		{{  0.5f, -0.5f,  0.5f }, {0, 0, 1}},
		{{  0.5f,  0.5f,  0.5f }, {0, 1, 0}},
		{{ -0.5f,  0.5f,  0.5f }, {1, 0, 0}},
		{{  0.5f, -0.5f, -0.5f }, {1, 0, 0}},
		{{ -0.5f, -0.5f, -0.5f }, {0, 1, 0}},
		{{ -0.5f,  0.5f, -0.5f }, {0, 0, 1}},
		{{  0.5f,  0.5f, -0.5f }, {0, 1, 0}},
	};
	std::vector<uint32_t> cube_indices = {
		// Front Face
		0, 1, 2,
		2, 3, 0,

		// Back Face
		4, 5, 6,
		6, 7, 4,

		// Left Face
		0, 3, 6,
		6, 5, 0,

		// Right Face
		2, 1, 4,
		4, 7, 2,

		// Top Face
		0, 5, 4,
		4, 1, 0,

		// Bottom Face
		3, 2, 7,
		7, 6, 3
	};
	scene_service->RegisterStaticMesh("cube", cube_vertices, cube_indices);
	create_game_scene1();





	// Startup Engine Services
	runtime_scheduler.Start();

	// Primary Exection Loop
	bool run_loop = true;
	float t = 0;
	while (run_loop)
	{
		// Check for window closure
		if (window_provider_ptr->should_close())
		{
			// Close Window
			// WindowProvider's destructor handles terminating the backend and destroying the window, so simply exit scope
			run_loop = false;
			break;
		}

		// Handle Window Events
		std::vector<vrender::platform::Event> events = window_provider_ptr->poll_events();
		for (vrender::platform::Event event : events)
		{
			// Process Event
			
			if (
				event.type == vrender::platform::EventType::KeyPress || 
				event.type == vrender::platform::EventType::KeyRelease
			)
			{
				// Pass Key Input Along to Input Engine
			}
			else if (
				event.type == vrender::platform::EventType::MouseButtonPress ||
				event.type == vrender::platform::EventType::MouseButtonRelease ||
				event.type == vrender::platform::EventType::MouseWheel
			)
			{
				// Pass Mouse Input Along to Input Engine
			}

			// Mutate Engine State
		}

		// Engine Tick
		runtime_scheduler.Tick(t);

		// Clear Resized Flag
		window_provider_ptr->clear_resize_flag();
		
		t += 0.01;
	}

	return 0;
}
*/

int main()
{
	std::shared_ptr<vrender::platform::WindowProvider> window_provider_ptr = std::make_shared<vrender::platform::GLFWWindowBackend>();
	std::shared_ptr<vrender::platform::WindowSurfaceProvider> surface_provider_ptr = std::dynamic_pointer_cast<
		vrender::platform::WindowSurfaceProvider
	>(window_provider_ptr);

	vrender::render::config::InstanceConfig instance_config{
		.enable_validation = ENABLE_VALIDATION_LAYERS,
	};
	//instance_config.extensions = std::vector<std::string>{};					// Not needed yet

	// Configure Window
	window_provider_ptr->set_title("VRENDER Engine");
	window_provider_ptr->set_resizable(true);

	vrender::render::Renderer renderer(
		*window_provider_ptr,
		*surface_provider_ptr,
		instance_config
	);

	std::vector<vrender::render::Vertex> cube_vertices = {
		{{ -0.5f, -0.5f,  0.5f }, {0, 1, 0}},
		{{  0.5f, -0.5f,  0.5f }, {0, 0, 1}},
		{{  0.5f,  0.5f,  0.5f }, {0, 1, 0}},
		{{ -0.5f,  0.5f,  0.5f }, {1, 0, 0}},
		{{  0.5f, -0.5f, -0.5f }, {1, 0, 0}},
		{{ -0.5f, -0.5f, -0.5f }, {0, 1, 0}},
		{{ -0.5f,  0.5f, -0.5f }, {0, 0, 1}},
		{{  0.5f,  0.5f, -0.5f }, {0, 1, 0}},
	};
	std::vector<uint32_t> cube_indices = {
		// Front Face
		0, 1, 2,
		2, 3, 0,

		// Back Face
		4, 5, 6,
		6, 7, 4,

		// Left Face
		0, 3, 6,
		6, 5, 0,

		// Right Face
		2, 1, 4,
		4, 7, 2,

		// Top Face
		0, 5, 4,
		4, 1, 0,

		// Bottom Face
		3, 2, 7,
		7, 6, 3
	};
	vrender::render::MeshHandle cube = renderer.get_geometry_arena().create_static_mesh(cube_vertices, cube_indices);


	while (true)
	{
		renderer.step({
			.objects = std::vector<vrender::render::RenderObject>{
				{
					.mesh = cube,
					.transform = ame::TRS(
						ame::vec3f{ 1.0f, -1.0f, -3.0f },
						ame::vec3f{ 0.0f, 0.0f, 0.0f },
						ame::vec3f{ 1.0f, 1.0f, 1.0f }
					)
				}
			}
		});
	}
}