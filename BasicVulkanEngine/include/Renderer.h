#ifndef RENDER_RENDERER_H
#define RENDER_RENDERER_H

#include <string>

#include <vulkan/vulkan.h>

#include <PlatformLayer/WindowBackends/WindowProvider.h>
#include <PlatformLayer/WindowBackends/WindowSurfaceProvider.h>

#include <CommandController.h>
#include <Fence.h>
#include <Framebuffer.h>
#include <ICommandRecorder.h>
#include <IFrameTarget.h>
#include <Instance.h>
#include <LogicalDevice.h>
#include <PhysicalDevice.h>
#include <RenderPass.h>
#include <RenderPassCommandRecorder.h>
#include <RenderPassFrameTarget.h>
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

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		// Public API
		bool step();
	private:
		const vrender::platform::WindowProvider& window_provider;
		const vrender::platform::WindowSurfaceProvider& window_surface_provider;

		vrender::render::Instance instance;
		VkSurfaceKHR surface;
		vrender::render::PhysicalDevice physical_device;
		vrender::render::LogicalDevice logical_device;
		vrender::render::Swapchain swapchain;
		
		vrender::render::Semaphore test_semaphore;
		std::vector<vrender::render::Fence> frame_fences;

		vrender::render::RenderPass render_pass;
		std::vector<vrender::render::Framebuffer> framebuffers;
		std::vector<std::unique_ptr<vrender::render::IFrameTarget>> frame_targets;
		std::vector<const vrender::render::IFrameTarget*> frame_targets_raw;
		std::unique_ptr<vrender::render::ICommandRecorder> command_recorder;

		vrender::render::CommandController command_controller;
	};
}

#endif