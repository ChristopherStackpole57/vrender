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

		// Indices
		VkDeviceSize index_offset;
		uint32_t index_count;
	};
}

#endif