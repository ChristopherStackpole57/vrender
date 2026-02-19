#include <iostream>

#include "Configuration/VulkanConfig.h"

#include <vulkan/vulkan.h>

#include <PlatformLayer/Utility/Event.h>
#include <PlatformLayer/Utility/WindowMode.h>
#include <PlatformLayer/WindowBackends/GLFWWindowBackend.h>

#include <Core/Renderer.h>
#include <Core/Mesh.h>
#include <Core/GeometryArena.h>

#include <RenderLayer/Configuration/InstanceConfiguration.h>

#ifdef NDEBUG
const bool ENABLE_VALIDATION_LAYERS = false;
#else
const bool ENABLE_VALIDATION_LAYERS = true;
#endif

int main()
{
	/*
	Create a shared pointer to a window provider, and from it generate a pointer to a surface provider.
	Because all window backends inherit from both the WindowProvider and WindowSurfaceProvider this pointer conversion is valid.
	Holding the pointer as window provider allows access to the WindowProvider API which will be interacted with in the primary
	execution loop. The surface provider, on the other hand, is necessary for generating a VkSurfaceKHR in the Instance constructor.
	*/

	std::shared_ptr<vrender::platform::WindowProvider> window_provider_ptr = std::make_shared<vrender::platform::GLFWWindowBackend>();
	std::shared_ptr<vrender::platform::WindowSurfaceProvider> surface_provider_ptr = std::dynamic_pointer_cast<
		vrender::platform::WindowSurfaceProvider
	>(window_provider_ptr);

	vrender::render::config::InstanceConfig instance_config{
		.enable_validation = ENABLE_VALIDATION_LAYERS,
	};
	//instance_config.extensions = std::vector<std::string>{};					// Not needed yet
	vrender::render::Renderer renderer(
		*window_provider_ptr,
		*surface_provider_ptr, 
		instance_config
	);

	// Configure Window
	window_provider_ptr->set_title("VRENDER Engine");
	window_provider_ptr->set_resizable(true);

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

		// Create Rotating Triangle
		const float c = std::cos(t);
		const float s = std::sin(t);
		std::vector<vrender::render::Vertex> vertices = {
			{{ -0.5f,  0.5f, 0.0f }, {1, 0, 0}},
			{{ -0.5f, -0.5f, 0.0f }, {0, 1, 0}},
			{{  0.5f,  0.5f, 0.0f }, {0, 1, 0}},
			{{  0.5f, -0.5f, 0.0f }, {0, 0, 1}}
		};
		std::vector<uint32_t> indices{
			0, 1, 2,
			1, 3, 2
		};
		
		for (vrender::render::Vertex& vertex : vertices)
		{
			float x = vertex.position[0];
			float y = vertex.position[1];

			vertex.position[0] = x * c - y * s;
			vertex.position[1] = x * s + y * c;
		}

		renderer.render_dynamic_mesh(
			vertices,
			indices
		);

		// Render Step
		if (!renderer.step()) return 0;

		// Clear Resized Flag
		window_provider_ptr->clear_resize_flag();
		
		t += 0.01;
	}

	return 0;
}