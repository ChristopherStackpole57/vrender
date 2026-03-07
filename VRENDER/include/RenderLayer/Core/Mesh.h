#ifndef RENDER_MESH_H
#define RENDER_MESH_H

#include <vulkan/vulkan.h>

#include <RenderLayer/Core/Memory/Buffer.h>

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

	struct MeshEntry
	{
		vrender::render::Mesh mesh;
	};
	struct MeshSlot
	{
		MeshEntry entry;
		uint32_t generation;
		bool alive;
	};
	typedef uint64_t MeshToken;
	struct MeshTokenComponents
	{
		uint32_t generation;
		uint32_t index;
	};
}

#endif