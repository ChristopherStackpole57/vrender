#include <RenderCycle/DynamicCommandRecorder.h>

// Lifetime Contorl
vrender::render::DynamicCommandRecorder::DynamicCommandRecorder(
	const vrender::render::Pipeline& pipeline,
	const vrender::render::GeometryArena& geometry_arena
)
	: pipeline_ptr(&pipeline)
	, geometry_arena_ptr(&geometry_arena)
{
	this->barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
	this->dependency_info.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
}

// API Accessibility
void vrender::render::DynamicCommandRecorder::begin(
	VkCommandBuffer command_buffer,
	const vrender::render::config::FrameDescription& config
)
{
	// Transition Image into Correct Format
	this->barrier.srcStageMask = VK_PIPELINE_STAGE_2_NONE;
	this->barrier.srcAccessMask = 0;
	this->barrier.dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
	this->barrier.dstAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;

	this->barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	this->barrier.newLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;

	this->barrier.image = config.swapchain_image;
	this->barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	this->barrier.subresourceRange.baseMipLevel = 0;
	this->barrier.subresourceRange.levelCount = 1;
	this->barrier.subresourceRange.baseArrayLayer = 0;
	this->barrier.subresourceRange.layerCount = 1;

	this->dependency_info.imageMemoryBarrierCount = 1;
	this->dependency_info.pImageMemoryBarriers = &this->barrier;

	vkCmdPipelineBarrier2(command_buffer, &this->dependency_info);

	// Start Command Binding from Config
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

	// Bind Data Buffers
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
	vkCmdBindDescriptorSets(
		command_buffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		this->pipeline_ptr->get_layout()->get_layout(),
		0,
		static_cast<uint32_t>(descriptor_sets.size()),
		descriptor_sets.data(),
		0,
		nullptr
	);

	uint32_t index = 0;
	for (const vrender::render::Mesh& mesh : meshes)
	{
		vkCmdPushConstants(
			command_buffer,
			this->pipeline_ptr->get_layout()->get_layout(),
			VK_SHADER_STAGE_VERTEX_BIT,
			0,
			sizeof(index),
			&index
		);
		index++;

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
void vrender::render::DynamicCommandRecorder::end(VkCommandBuffer command_buffer)
{
	vkCmdEndRendering(command_buffer);

	// Transition Swapchain Image Back to Present
	this->barrier.srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
	this->barrier.srcAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
	this->barrier.dstStageMask = VK_PIPELINE_STAGE_2_NONE;
	this->barrier.dstAccessMask = 0;

	this->barrier.oldLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
	this->barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	this->dependency_info.imageMemoryBarrierCount = 1;
	this->dependency_info.pImageMemoryBarriers = &barrier;

	vkCmdPipelineBarrier2(command_buffer, &this->dependency_info);
}