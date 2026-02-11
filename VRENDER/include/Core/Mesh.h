#ifndef RENDER_MESH_H
#define RENDER_MESH_H

#include <vulkan/vulkan.h>

#include <Core/Memory/Buffer.h>

namespace vrender::render
{
	struct Mesh
	{
		// Vertex Buffer
		vrender::render::memory::Buffer* vertex_buffer;
		VkDeviceSize vertex_offset;

		// Index Buffer
		vrender::render::memory::Buffer* index_buffer;
		VkDeviceSize index_offset;
		uint32_t index_count;

		// Binding
		void bind(VkCommandBuffer command_bufer) const;
	};
}

#endif