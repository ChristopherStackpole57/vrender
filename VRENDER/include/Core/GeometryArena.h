#ifndef RENDER_GEOMETRY_ARENA_H
#define RENDER_GEOMETRY_ARENA_H

#include <vulkan/vulkan.h>

#include <Core/Mesh.h>

#include <Core/Memory/Allocator.h>
#include <Core/Memory/Buffer.h>
#include <Core/Memory/BufferDesc.h>
#include <Core/Memory/Suballocator.h>

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
		GeometryArena(vrender::render::memory::Allocator& allocator, uint32_t dynamic_divisions);
		~GeometryArena();

		GeometryArena(const GeometryArena&) = delete;
		GeometryArena& operator=(const GeometryArena&) = delete;

		GeometryArena(GeometryArena&& other) noexcept = default;
		GeometryArena& operator=(GeometryArena&& other) noexcept = default;

		// API Accessibility
		const vrender::render::Mesh create_static_mesh(
			std::vector<vrender::render::Vertex>& vertices,
			std::vector<uint32_t>& indices	
		);
		const vrender::render::Mesh create_dynamic_mesh(
			std::vector<vrender::render::Vertex>& vertices,
			std::vector<uint32_t>& indices,
			uint32_t index
		);

		void reset_dynamic(uint32_t index);

		const vrender::render::memory::Buffer& get_vertex_buffer() const;
		const vrender::render::memory::Buffer& get_index_buffer() const;
	private:
		uint32_t static_arena_length = 1024;
		uint32_t dynamic_arena_total_length = 512;

		vrender::render::memory::Buffer vertex_buffer;
		vrender::render::memory::Buffer index_buffer;

		vrender::render::memory::Suballocator static_vertex_suballocator;
		vrender::render::memory::Suballocator static_index_suballocator;
		std::vector<vrender::render::memory::Suballocator> dynamic_vertex_suballocators;
		std::vector<vrender::render::memory::Suballocator> dynamic_index_suballocators;
	};
}

#endif