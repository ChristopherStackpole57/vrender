#ifndef RENDER_RENDERER_H
#define RENDER_RENDERER_H

#include <string>

#include <vulkan/vulkan.h>

#include <PlatformLayer/WindowBackends/WindowProvider.h>
#include <PlatformLayer/WindowBackends/WindowSurfaceProvider.h>

#include <Framebuffer.h>
#include <Instance.h>
#include <LogicalDevice.h>
#include <PhysicalDevice.h>
#include <RenderPass.h>
#include <Semaphore.h>
#include <Swapchain.h>

#include <RenderLayer/Configuration/InstanceConfiguration.h>
#include <RenderLayer/Configuration/RenderPassConfiguration.h>

#include <RenderLayer/Misc/BaseRenderPassConfigs.cpp>

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
			const vrender::render::config::InstanceConfig& instance_config
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

		std::vector<std::unique_ptr<vrender::render::RenderPass>> render_passes;
		std::vector<std::unique_ptr<vrender::render::Framebuffer>> framebuffers;

		vrender::render::Semaphore test_semaphore;
	};
}

#endif