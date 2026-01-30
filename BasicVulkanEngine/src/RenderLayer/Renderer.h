#ifndef RENDER_RENDERER_H
#define RENDER_RENDERER_H

#include <string>

#include <PlatformLayer/WindowBackends/WindowProvider.h>
#include <PlatformLayer/WindowBackends/WindowSurfaceProvider.h>

#include <RenderLayer/InstanceConfiguration.h>
#include <RenderLayer/Instance.h>
#include <RenderLayer/PhysicalDevice.h>
#include <RenderLayer/LogicalDevice.h>
#include <RenderLayer/Semaphore.h>
#include <RenderLayer/Swapchain.h>

#include <RenderLayer/Utility/PhysicalDeviceEnumeration.h>
#include <RenderLayer/Utility/QuerySwapchainSupport.h>
#include <RenderLayer/Utility/QueueSelection.h>
#include <RenderLayer/Utility/SelectPhysicalDevice.h>

namespace vrender::render
{
	class Renderer
	{
	public:
		// Lifetime Control
		Renderer(
			const vrender::platform::WindowProvider& window_provider,
			const vrender::platform::WindowSurfaceProvider& surface_provider, 
			const vrender::render::InstanceConfig& instance_config
		);
		~Renderer();

		// Public API
		void step(const vrender::platform::WindowProvider& window);
	private:
		vrender::render::Instance instance;
		VkSurfaceKHR surface;
		vrender::render::PhysicalDevice physical_device;
		vrender::render::LogicalDevice logical_device;
		vrender::render::Swapchain swapchain;

		vrender::render::Semaphore test_semaphore;
	};
}

#endif