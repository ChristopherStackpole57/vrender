#ifndef RENDER_GEOMETRY_ARENA_H
#define RENDER_GEOMETRY_ARENA_H

#include <vulkan/vulkan.h>

#include <RenderLayer/Core/Mesh.h>
#include <RenderLayer/Core/Vertex.h>

#include <RenderLayer/Core/Memory/Allocator.h>
#include <RenderLayer/Core/Memory/Buffer.h>
#include <RenderLayer/Core/Memory/BufferDesc.h>
#include <RenderLayer/Core/Memory/Suballocator.h>

namespace vrender::render
{
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
		const vrender::render::MeshToken create_static_mesh(
			std::vector<vrender::render::Vertex>& vertices,
			std::vector<uint32_t>& indices	
		);
		const vrender::render::MeshToken create_dynamic_mesh(
			std::vector<vrender::render::Vertex>& vertices,
			std::vector<uint32_t>& indices,
			uint32_t index
		);

		void reset_dynamic(uint32_t index);

		const vrender::render::memory::Buffer& get_vertex_buffer() const;
		const vrender::render::memory::Buffer& get_index_buffer() const;

		const vrender::render::Mesh get_mesh(vrender::render::MeshToken token) const;
	private:
		// Geometry Suballocation
		uint32_t static_arena_length = 1024;
		uint32_t dynamic_arena_total_length = 512;

		vrender::render::memory::Buffer vertex_buffer;
		vrender::render::memory::Buffer index_buffer;

		vrender::render::memory::Suballocator static_vertex_suballocator;
		vrender::render::memory::Suballocator static_index_suballocator;
		std::vector<vrender::render::memory::Suballocator> dynamic_vertex_suballocators;
		std::vector<vrender::render::memory::Suballocator> dynamic_index_suballocators;

		// Handle Management
		vrender::render::MeshToken encode_token(uint64_t index, uint64_t generation) const;
		vrender::render::MeshTokenComponents decode_token(vrender::render::MeshToken token) const;
		vrender::render::MeshToken acquire_slot_token(vrender::render::MeshEntry entry);
		vrender::render::MeshSlot& slot_from_token(vrender::render::MeshToken token);

		bool token_valid(vrender::render::MeshToken token) const;
		bool token_alive(vrender::render::MeshToken token) const;

		std::vector<vrender::render::MeshSlot> slots;
		std::vector<uint32_t> free_indices;
	};
}

#endif