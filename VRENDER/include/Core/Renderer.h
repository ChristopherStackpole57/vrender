#ifndef RENDER_RENDERER_H
#define RENDER_RENDERER_H

#include <string>

#include <vulkan/vulkan.h>

#include <PlatformLayer/WindowBackends/WindowProvider.h>
#include <PlatformLayer/WindowBackends/WindowSurfaceProvider.h>

#include <Core/CommandController.h>
#include <Core/FrameContext.h>
#include <Core/GeometryArena.h>
#include <Core/ICommandRecorder.h>
#include <Core/IDescriptorController.h>
#include <Core/IFrameTarget.h>
#include <Core/Instance.h>
#include <Core/LogicalDevice.h>
#include <Core/PhysicalDevice.h>
#include <Core/Shader.h>
#include <Core/Swapchain.h>

#include <Core/Memory/Allocator.h>

#include <LegacyRender/DescriptorLayout.h>
#include <LegacyRender/DescriptorPool.h>
#include <LegacyRender/Framebuffer.h>
#include <LegacyRender/Pipeline.h>
#include <LegacyRender/PipelineLayout.h>
#include <LegacyRender/RenderPass.h>
#include <LegacyRender/RenderPassCommandRecorder.h>
#include <LegacyRender/RenderPassDescriptorController.h>
#include <LegacyRender/RenderPassFrameTarget.h>

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
			const vrender::render::config::InstanceConfig& instance_config,
			const uint32_t max_frames = 3
		);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		// Public API
		bool step();

		vrender::render::GeometryArena& get_geometry_arena();
		void add_mesh(vrender::render::Mesh mesh);
	private:
		const vrender::platform::WindowProvider& window_provider;
		const vrender::platform::WindowSurfaceProvider& window_surface_provider;

		std::vector<std::string> required_extensions = { 
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME
		};

		vrender::render::Instance instance;
		VkSurfaceKHR surface;
		vrender::render::PhysicalDevice physical_device;
		vrender::render::LogicalDevice logical_device;
		vrender::render::Swapchain swapchain;
		
		// Memory
		vrender::render::memory::Allocator allocator;

		std::vector<vrender::render::FrameContext> frame_contexts;
		uint32_t MAX_FRAMES_IN_FLIGHT = 3;
		uint32_t current_frame = 0;

		// Render Model Specific
		vrender::render::RenderPass render_pass;

		std::vector<vrender::render::Framebuffer> framebuffers;
		std::vector<std::unique_ptr<vrender::render::IFrameTarget>> frame_targets;
		std::vector<const vrender::render::IFrameTarget*> frame_targets_raw;

		std::unique_ptr<vrender::render::ICommandRecorder> command_recorder;

		vrender::render::Shader vertex;
		vrender::render::Shader fragment;

		std::vector<vrender::render::DescriptorLayout> descriptor_layouts;
		std::unique_ptr<vrender::render::IDescriptorController> descriptor_controller;
		std::vector<VkPushConstantRange> push_constants = {};

		vrender::render::PipelineLayout pipeline_layout;
		vrender::render::Pipeline pipeline;

		// Generic Render
		vrender::render::DescriptorPool persistent_descriptor_pool;
		vrender::render::CommandController command_controller;
		vrender::render::GeometryArena geometry_arena;

		std::vector<vrender::render::Mesh> meshes;
	};
}

#endif