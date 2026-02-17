#include <RednerCycle/DynamicCommandRecorder.h>

void vrender::render::DynamicCommandRecorder::begin(VkCommandBuffer command_buffer)
{
	VkRenderingInfo rendering_info{};
	rendering_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
	//rendering_info.pnext = &pipeline_rendering_create_info;

	// set viewport
	// set scissor

	vkCmdBeginRendering(
		command_buffer,
		&rendering_info
	);
}
void vrender::render::DynamicCommandRecorder::record(
	const VkCommandBuffer command_buffer,
	const vrender::render::IFrameTarget& frame_target,
	const std::vector<VkDescriptorSet> descriptor_sets,
	const std::vector<vrender::render::Mesh>& meshes
)
{
	// draw commands
}
void vrender::render::DynamicCommandRecorder::end(VkCommandBuffer command_buffer)
{
	vkCmdEndRendering();
}