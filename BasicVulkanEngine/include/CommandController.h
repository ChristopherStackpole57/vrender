#ifndef RENDER_COMMAND_CONTROLLER_H
#define RENDER_COMMAND_CONTROLLER_H

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include <Framebuffer.h>
#include <FrameContext.h>
#include <ICommandRecorder.h>
#include <IFrameTarget.h>
#include <LogicalDevice.h>
#include <Swapchain.h>

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
			const vrender::render::ICommandRecorder& command_recorder,
			const std::vector<const vrender::render::IFrameTarget*> frame_targets
		);
		~CommandController();

		CommandController(const CommandController&) = delete;
		CommandController& operator=(const CommandController&) = delete;

		CommandController(CommandController&& other) noexcept;
		CommandController& operator=(CommandController&& other) noexcept;

		// API Accessibility
		void record(uint32_t frame_index);
		void submit(uint32_t frame_index, vrender::render::FrameContext& frame_context);
		void present(uint32_t frame_index, vrender::render::FrameContext& frame_context);
	private:
		const vrender::render::LogicalDevice* logical_device_ptr;
		// TODO: Change name to swapchain ptr
		const vrender::render::Swapchain* swapchain;
		const vrender::render::ICommandRecorder* command_recorder;

		std::vector<VkCommandPool> command_pools;
		std::vector<VkCommandBuffer> command_buffers;
		std::vector<const vrender::render::IFrameTarget*> frame_targets;
	};
}

#endif 