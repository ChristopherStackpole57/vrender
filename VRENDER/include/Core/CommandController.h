#ifndef RENDER_COMMAND_CONTROLLER_H
#define RENDER_COMMAND_CONTROLLER_H

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include <Core/FrameContext.h>
#include <Core/ICommandRecorder.h>
#include <Core/IDescriptorController.h>
#include <Core/IFrameTarget.h>
#include <Core/LogicalDevice.h>
#include <Core/Mesh.h>
#include <Core/Swapchain.h>

#include <Core/DescriptorLayout.h>
#include <Core/DescriptorPool.h>

#include <Core/PipelineLayout.h>

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
			const vrender::render::ICommandRecorder* command_recorder,
			const vrender::render::IDescriptorController* descriptor_controller,
			const std::vector<const vrender::render::IFrameTarget*> frame_targets
		);
		~CommandController();

		CommandController(const CommandController&) = delete;
		CommandController& operator=(const CommandController&) = delete;

		CommandController(CommandController&& other) noexcept;
		CommandController& operator=(CommandController&& other) noexcept;

		// API Accessibility
		void record(
			uint32_t frame_index,
			vrender::render::FrameContext& frame_context,
			vrender::render::FrameDescriptorInputs inputs,
			std::vector<vrender::render::Mesh>& meshes
		);
		void submit(uint32_t frame_index, vrender::render::FrameContext& frame_context);
		void present(uint32_t image_index, vrender::render::FrameContext& frame_context);
	private:
		const vrender::render::LogicalDevice* logical_device_ptr;
		const vrender::render::Swapchain* swapchain_ptr;
		const vrender::render::ICommandRecorder* command_recorder;
		const vrender::render::IDescriptorController* descriptor_controller;

		std::vector<VkCommandPool> command_pools;
		std::vector<VkCommandBuffer> command_buffers;
		std::vector<vrender::render::DescriptorPool> descriptor_pools;
		std::vector<const vrender::render::IFrameTarget*> frame_targets;
	};
}

#endif 