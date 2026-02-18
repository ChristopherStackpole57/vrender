#include <RenderCycle/DynamicCommandRecorder.h>

// Lifetime Contorl
vrender::render::DynamicCommandRecorder::DynamicCommandRecorder(
	const vrender::render::Pipeline& pipeline,
	const vrender::render::GeometryArena& geometry_arena
)
	: pipeline_ptr(&pipeline)
	, geometry_arena_ptr(&geometry_arena)
{

}

// API Accessibility
void vrender::render::DynamicCommandRecorder::begin(
	VkCommandBuffer command_buffer,
	const vrender::render::config::FrameDescription& config
) const
{
	VkClearValue clear_color{};
	clear_color.color = { config.clear_color };

	VkRenderingAttachmentInfo color_attachment{};
	color_attachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
	color_attachment.imageView = config.swapchain_image_view;
	color_attachment.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment.clearValue = clear_color;

	VkRenderingInfo rendering_info{};
	rendering_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
	rendering_info.renderArea.offset = { 0, 0 };
	rendering_info.renderArea.extent = config.extent;
	rendering_info.layerCount = 1;
	rendering_info.colorAttachmentCount = 1;
	rendering_info.pColorAttachments = &color_attachment;

	vkCmdBeginRendering(
		command_buffer,
		&rendering_info
	);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(config.extent.width);
	viewport.height = static_cast<float>(config.extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	vkCmdSetViewport(
		command_buffer,
		0,
		1,
		&viewport
	);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = config.extent;

	vkCmdSetScissor(
		command_buffer,
		0,
		1,
		&scissor
	);
}
void vrender::render::DynamicCommandRecorder::record(
	const VkCommandBuffer command_buffer,
	const std::vector<VkDescriptorSet> descriptor_sets,
	const std::vector<vrender::render::Mesh>& meshes
) const
{
	vkCmdBindPipeline(
		command_buffer,
		this->pipeline_ptr->get_bind_point(),
		this->pipeline_ptr->get_pipeline()
	);

	VkBuffer vertex_buffer = this->geometry_arena_ptr->get_vertex_buffer().get_buffer();
	VkBuffer index_buffer = this->geometry_arena_ptr->get_index_buffer().get_buffer();
	VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers(
		command_buffer,
		0,
		1,
		&vertex_buffer,
		&offset
	);
	vkCmdBindIndexBuffer(
		command_buffer,
		index_buffer,
		offset,
		VK_INDEX_TYPE_UINT32
	);

	// handle descriptor sets; currently bound, will eventually be bindless// bind descriptors

	for (const vrender::render::Mesh& mesh : meshes)
	{
		vkCmdDrawIndexed(
			command_buffer,
			mesh.index_count,
			1,
			mesh.index_offset_count,
			mesh.vertex_offset_count,
			0
		);
	}
}
void vrender::render::DynamicCommandRecorder::end(VkCommandBuffer command_buffer) const
{
	vkCmdEndRendering(command_buffer);
}