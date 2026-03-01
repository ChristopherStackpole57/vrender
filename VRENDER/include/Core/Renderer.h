#ifndef RENDER_RENDERER_H
#define RENDER_RENDERER_H

#include <string>

#include <vulkan/vulkan.h>

#include <ame/AME.h>

#include <PlatformLayer/WindowBackends/WindowProvider.h>
#include <PlatformLayer/WindowBackends/WindowSurfaceProvider.h>

#include <Core/CommandController.h>
#include <Core/FrameContext.h>
#include <Core/GeometryArena.h>
#include <Core/ICommandRecorder.h>
#include <Core/IDescriptorController.h>
#include <Core/Instance.h>
#include <Core/LogicalDevice.h>
#include <Core/PhysicalDevice.h>
#include <Core/Pipeline.h>
#include <Core/PipelineLayout.h>
#include <Core/Shader.h>
#include <Core/Swapchain.h>

#include <Core/Memory/Allocator.h>

#include <Core/DescriptorLayout.h>
#include <Core/DescriptorPool.h>
#include <Core/DescriptorSet.h>

#include <RenderCycle/BindlessRegistry.h>
#include <RenderCycle/DynamicCommandRecorder.h>

#include <RenderLayer/Configuration/InstanceConfiguration.h>
#include <RenderLayer/Configuration/FrameDescription.h>

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
		void render_dynamic_mesh(std::vector<vrender::render::Vertex> vertices, std::vector<uint32_t> indices);
	private:
		// Utility
		void regenerate_swapchain();

		// Basic
		const vrender::platform::WindowProvider& window_provider;
		const vrender::platform::WindowSurfaceProvider& window_surface_provider;
		
		uint32_t MAX_FRAMES_IN_FLIGHT = 3;
		uint32_t current_frame = 0;

		std::vector<std::string> required_extensions = { 
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
			VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
			VK_KHR_DYNAMIC_RENDERING_LOCAL_READ_EXTENSION_NAME,
			VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME
		};

		// Core
		vrender::render::Instance instance;
		VkSurfaceKHR surface;
		vrender::render::PhysicalDevice physical_device;
		vrender::render::LogicalDevice logical_device;
		vrender::render::Swapchain swapchain;
		


		// Memory
		vrender::render::memory::Allocator allocator;
		vrender::render::GeometryArena geometry_arena;



		// Render Cycle Specific
		std::vector<vrender::render::Mesh> meshes;
		std::vector<std::vector<vrender::render::Vertex>> pending_dynamic_vertices;
		std::vector<std::vector<uint32_t>> pending_dynamic_indices;

		std::vector<vrender::render::FrameContext> frame_contexts;
		std::vector<VkFence> images_in_flight;

		vrender::render::Shader vertex;
		vrender::render::Shader fragment;

		std::vector<vrender::render::DescriptorLayout> descriptor_layouts;
		std::vector<VkPushConstantRange> push_constants = {};

		vrender::render::DescriptorPool persistent_descriptor_pool;
		vrender::render::BindlessRegistry bindless_registry;

		std::unique_ptr<vrender::render::IDescriptorController> descriptor_controller;

		vrender::render::PipelineLayout pipeline_layout;
		vrender::render::Pipeline pipeline;

		std::unique_ptr<vrender::render::ICommandRecorder> command_recorder;

		vrender::render::CommandController command_controller;



		// Testing
		vrender::render::memory::Buffer test_transform_buffer;
		vrender::render::BRToken transform_buffer_token;
		uint32_t time = 0;

		vrender::render::memory::Buffer test_camera_buffer;
		vrender::render::BRToken camera_buffer_token;
	};
}

#endif