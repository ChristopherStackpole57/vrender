#ifndef RENDER_RENDER_PASS_COMMAND_RECORDER_H
#define RENDER_RENDER_PASS_COMMAND_RECORDER_H

#include <vulkan/vulkan.h>

#include <Core/ICommandRecorder.h>

#include <LegacyRender/Pipeline.h>

namespace vrender::render
{
	class RenderPassCommandRecorder : public vrender::render::ICommandRecorder
	{
	public:
		// Lifetime Control
		RenderPassCommandRecorder(
			const vrender::render::Pipeline& pipeline
		);
		~RenderPassCommandRecorder();

		// API Accessibility
		void record(
			const VkCommandBuffer command_buffer,
			const vrender::render::IFrameTarget& frame_target
		) const override;
	private:
		const vrender::render::Pipeline& pipeline;
	};
}

#endif