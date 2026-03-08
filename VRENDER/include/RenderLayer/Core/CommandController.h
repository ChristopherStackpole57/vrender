#ifndef RENDER_COMMAND_CONTROLLER_H
#define RENDER_COMMAND_CONTROLLER_H

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include <RenderLayer/Core/DescriptorLayout.h>
#include <RenderLayer/Core/DescriptorPool.h>
#include <RenderLayer/Core/DescriptorSet.h>
#include <RenderLayer/Core/FrameContext.h>
#include <RenderLayer/Core/ICommandRecorder.h>
#include <RenderLayer/Core/IDescriptorController.h>
#include <RenderLayer/Core/LogicalDevice.h>
#include <RenderLayer/Core/Mesh.h>
#include <RenderLayer/Core/PipelineLayout.h>
#include <RenderLayer/Core/Swapchain.h>

#include <RenderLayer/Configuration/FrameDescription.h>

namespace vrender::render
{
	class CommandController
	{
	public:
		// Lifetime Control
		CommandController(
			const vrender::render::LogicalDevice& logical_device,
			const uint32_t queue_family_index,
			const vrender::render::Swapchain& swapchain,
			vrender::render::ICommandRecorder* command_recorder,
			const vrender::render::IDescriptorController* descriptor_controller,
			const uint32_t max_frames_in_flight
		);
		~CommandController();

		CommandController(const CommandController&) = delete;
		CommandController& operator=(const CommandController&) = delete;

		CommandController(CommandController&& other) noexcept;
		CommandController& operator=(CommandController&& other) noexcept;

		// API Accessibility
		void record(
			uint32_t frame_index,
			const vrender::render::config::FrameDescription& frame_description,
			const std::vector<VkDescriptorSet>& descriptor_sets,
			std::vector<vrender::render::Mesh>& meshes
		);
		void submit(uint32_t frame_index, vrender::render::FrameContext& frame_context);
		void present(uint32_t image_index, vrender::render::FrameContext& frame_context);
	private:
		const vrender::render::LogicalDevice* logical_device_ptr;
		const vrender::render::Swapchain* swapchain_ptr;
		vrender::render::ICommandRecorder* command_recorder;
		const vrender::render::IDescriptorController* descriptor_controller;

		std::vector<VkCommandPool> command_pools;
		std::vector<VkCommandBuffer> command_buffers;
	};
}

#endif 