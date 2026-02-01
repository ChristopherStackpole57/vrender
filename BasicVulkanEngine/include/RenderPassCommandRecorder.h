#ifndef RENDER_RENDER_PASS_COMMAND_RECORDER_H
#define RENDER_RENDER_PASS_COMMAND_RECORDER_H

#include <vulkan/vulkan.h>

#include <ICommandRecorder.h>

namespace vrender::render
{
	class RenderPassCommandRecorder : public vrender::render::ICommandRecorder
	{
	public:
		void record(
			const VkCommandBuffer command_buffer,
			const vrender::render::IFrameTarget& frame_target
		) const override;
	private:
	};
}

#endif