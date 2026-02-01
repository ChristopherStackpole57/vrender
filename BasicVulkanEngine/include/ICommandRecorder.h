#ifndef RENDER_INTERFACE_COMMAND_RECORDER_H
#define RENDER_INTERFACE_COMMAND_RECORDER_H

#include <vulkan/vulkan.h>

#include <IFrameTarget.h>

namespace vrender::render
{
	class ICommandRecorder
	{
	public:
		virtual ~ICommandRecorder() = default;

		virtual void record(
			const VkCommandBuffer command_buffer, 
			const vrender::render::IFrameTarget& frame_target
		) const = 0;
	};
}

#endif