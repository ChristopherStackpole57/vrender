#include <iostream>

#include "Configuration/VulkanConfig.h"

#include <vulkan/vulkan.h>

#include <PlatformLayer/Utility/Event.h>
#include <PlatformLayer/Utility/WindowMode.h>
#include <PlatformLayer/WindowBackends/GLFWWindowBackend.h>

#include <RenderLayer/InstanceConfiguration.h>
#include <RenderLayer/Renderer.h>

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

	vrender::render::InstanceConfig instance_config{
		.enable_validation = ENABLE_VALIDATION_LAYERS,
	};
	//instance_config.extensions = std::vector<std::string>{};					// Not needed yet
	vrender::render::Renderer renderer(
		*window_provider_ptr,
		*surface_provider_ptr, 
		instance_config
	);

	// Configure Window
	window_provider_ptr->set_title("Platform Agnostic Window");
	window_provider_ptr->set_resizable(true);

	// Primary Exection Loop
	bool run_loop = true;
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

			// Render Step
			renderer.step(*window_provider_ptr);
		}
	}

	return 0;
}