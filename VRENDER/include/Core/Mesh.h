#ifndef RENDER_MESH_H
#define RENDER_MESH_H

#include <vulkan/vulkan.h>

#include <Core/Memory/Buffer.h>

namespace vrender::render
{
	struct Mesh
	{
		// Vertices
		VkDeviceSize vertex_offset;
		VkDeviceSize vertex_offset_count;
		uint32_t vertex_count;

		// Indices
		VkDeviceSize index_offset;
		VkDeviceSize index_offset_count;
		uint32_t index_count;
	};
}

#endif