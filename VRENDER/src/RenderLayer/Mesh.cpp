#include <Core/Mesh.h>

void vrender::render::Mesh::bind(VkCommandBuffer command_buffer) const
{
	// Force Index Buffer
	if (!index_buffer)
	{
		throw std::runtime_error("ERROR: Mesh is Required To Have Valid Index Buffer");
	}

	VkDeviceSize offsets[] = { vertex_offset };
	VkBuffer vert = vertex_buffer->get_buffer();
	vkCmdBindVertexBuffers(
		command_buffer,
		0,
		1,
		&vert,
		offsets
	);

	vkCmdBindIndexBuffer(
		command_buffer,
		index_buffer->get_buffer(),
		index_offset,
		VK_INDEX_TYPE_UINT32
	);
}