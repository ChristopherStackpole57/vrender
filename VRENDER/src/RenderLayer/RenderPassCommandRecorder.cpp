#include <LegacyRender/RenderPassCommandRecorder.h>

// Lifetime Control
vrender::render::RenderPassCommandRecorder::RenderPassCommandRecorder(
	const vrender::render::LogicalDevice& logical_device,
	const vrender::render::PhysicalDevice& physical_device,
	const vrender::render::Pipeline& pipeline,
	const vrender::render::GeometryArena& geometry_arena
)
	: pipeline(pipeline)
	, logical_device_ptr(&logical_device)
	, physical_device_ptr(&physical_device)
	, geometry_arena_ptr(&geometry_arena)
{

}
vrender::render::RenderPassCommandRecorder::~RenderPassCommandRecorder()
{

}

// API Accessibility
void vrender::render::RenderPassCommandRecorder::record(
	const VkCommandBuffer command_buffer,
	const vrender::render::IFrameTarget& frame_target,
	const std::vector<VkDescriptorSet> descriptor_sets,
	const std::vector<vrender::render::Mesh>& meshes
) const
{
	// Bind Pipeline
	vkCmdBindPipeline(
		command_buffer,
		this->pipeline.get_bind_point(),
		this->pipeline.get_pipeline()
	);

	// Bind GeometryArena Vertex Buffer and Index Buffer
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
		0,
		VK_INDEX_TYPE_UINT32
	);

	// Bind Sets
	if (descriptor_sets.size() > 0)
	{
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
	}

	// Push Constants
	//vkCmdPushConstants();

	// Draw Meshes
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