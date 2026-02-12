#ifndef RENDER_GEOMETRY_ARENA_H
#define RENDER_GEOMETRY_ARENA_H

#include <vulkan/vulkan.h>

#include <Core/Mesh.h>

#include <Core/Memory/Buffer.h>
#include <Core/Memory/BufferDesc.h>

namespace vrender::render
{
	struct Vertex
	{
		float position[3];
		// Normal
		// Tangent
		float color[3];
	};

	class GeometryArena
	{
	public:
		// Lifetime Control
		GeometryArena(uint32_t dynamic_divisions);
		~GeometryArena();

		// API Accessibility
		const vrender::render::Mesh create_static_mesh(std::vector<vrender::render::Vertex>& vertices);
		const vrender::render::Mesh create_dynamic_mesh(std::vector<vrender::render::Vertex>& vertices);

		void reset_dynamic(uint32_t index);
	private:
		vrender::render::memory::Buffer vertex_buffer;
		vrender::render::memory::Buffer index_buffer;

		uint32_t static_arena_length = 1024;
		uint32_t dynamic_arena_total_length = 512;
	};
}

#endif