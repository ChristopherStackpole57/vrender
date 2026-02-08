#ifndef RENDER_INTERFACE_COMMAND_RECORDER_H
#define RENDER_INTERFACE_COMMAND_RECORDER_H

#include <vector>

#include <vulkan/vulkan.h>

#include <Core/IFrameTarget.h>

namespace vrender::render
{
	class ICommandRecorder
	{
	public:
		virtual ~ICommandRecorder() = default;

		virtual void record(
			const VkCommandBuffer command_buffer, 
			const vrender::render::IFrameTarget& frame_target,
			const std::vector<VkDescriptorSet> descriptor_set
		) const = 0;
	};
}

#endif