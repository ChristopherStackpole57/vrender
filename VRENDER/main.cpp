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
#include <EngineLayer/Core/ServiceManager.h>

#ifdef NDEBUG
const bool ENABLE_VALIDATION_LAYERS = false;
#else
const bool ENABLE_VALIDATION_LAYERS = true;
#endif

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
	runtime_scheduler.SetServiceTickPriority(
		vrender::engine::Services().Get<vrender::engine::RenderService>(),
		vrender::engine::SCHEDULE_TICK_LEVEL_FRAMERENDER
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
	vrender::render::MeshToken token = vrender::engine::Services().Get<vrender::engine::RenderService>()->CreateMesh(
		cube_vertices,
		cube_indices
	);





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

		vrender::engine::Services().Get<vrender::engine::RenderService>()
			->SubmitRenderObject(
				vrender::render::RenderObject{
					.mesh = token,
					.transform = ame::TRS(
						ame::vec3f{  1.0f, - 1.0f, - 4.0f },
						ame::vec3f{ 40.0f,  25.0f,  15.0f },
						ame::vec3f{  1.0f,   1.0f,   1.0f }
					)
				}
		);

		// Engine Tick
		runtime_scheduler.Tick(t);

		// Clear Resized Flag
		window_provider_ptr->clear_resize_flag();
		
		t += 0.01;
	}

	return 0;
}