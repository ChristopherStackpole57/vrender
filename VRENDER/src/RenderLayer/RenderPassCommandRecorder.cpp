#include <LegacyRender/RenderPassCommandRecorder.h>

// Lifetime Control
vrender::render::RenderPassCommandRecorder::RenderPassCommandRecorder(
	const vrender::render::LogicalDevice& logical_device,
	const vrender::render::PhysicalDevice& physical_device,
	const vrender::render::Pipeline& pipeline
)
	: pipeline(pipeline)
	, logical_device_ptr(&logical_device)
	, physical_device_ptr(&physical_device)
{

}
vrender::render::RenderPassCommandRecorder::~RenderPassCommandRecorder()
{

}

// API Accessibility
void vrender::render::RenderPassCommandRecorder::record(
	const VkCommandBuffer command_buffer,
	const vrender::render::IFrameTarget& frame_target,
	const std::vector<VkDescriptorSet> descriptor_sets
) const
{
	// Bind Pipeline
	vkCmdBindPipeline(
		command_buffer,
		this->pipeline.get_bind_point(),
		this->pipeline.get_pipeline()
	);

	// Bind Sets
	vkCmdBindDescriptorSets(
		command_buffer,
		this->pipeline.get_bind_point(),
		this->pipeline.get_layout()->get_layout(),
		0,
		static_cast<uint32_t>(descriptor_sets.size()),
		descriptor_sets.data(),
		0,
		nullptr
	);

	// Push Constants
	//vkCmdPushConstants();

	// Draw
	vkCmdDraw(
		command_buffer,
		6,
		1,
		0,
		0
	);
}