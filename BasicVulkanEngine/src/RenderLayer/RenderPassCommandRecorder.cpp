#include <RenderPassCommandRecorder.h>

// Lifetime Control
vrender::render::RenderPassCommandRecorder::RenderPassCommandRecorder(
	const vrender::render::Pipeline& pipeline
)
	: pipeline(pipeline)
{

}
vrender::render::RenderPassCommandRecorder::~RenderPassCommandRecorder()
{

}

// API Accessibility
void vrender::render::RenderPassCommandRecorder::record(
	const VkCommandBuffer command_buffer,
	const vrender::render::IFrameTarget& frame_target
) const
{
	// No code necessary yet,
	// This will eventually get filled with a pipeline, descriptor set, etc.

	// Bind Pipeline
	vkCmdBindPipeline(
		command_buffer,
		this->pipeline.get_bind_point(),
		this->pipeline.get_pipeline()
	);

	// Bind Descriptor Sets
	/*vkCmdBindDescriptorSets(
		command_buffer,
		this->pipeline.get_bind_point(),
		this->pipeline.get_layout(),
		0,
		0,
		nullptr,
		0,
		nullptr
	);*/

	// Push Constants
	//vkCmdPushConstants();

	// Draw
	vkCmdDraw(
		command_buffer,
		3,
		1,
		0,
		0
	);
}