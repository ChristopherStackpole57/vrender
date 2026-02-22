#include <Core/GeometryArena.h>

// Lifetime Control
vrender::render::GeometryArena::GeometryArena(
	vrender::render::memory::Allocator& allocator,
	uint32_t dynamic_divisions
)
	: vertex_buffer(
		allocator,
		static_cast<size_t>(
			(this->static_arena_length + this->dynamic_arena_total_length) *
			sizeof(vrender::render::Vertex)
		),
		vrender::render::memory::BufferUsageClass::VERTEX,
		vrender::render::memory::BufferCPUAccess::WRITE_OFTEN,
		vrender::render::memory::BufferLifetime::PERSISTENT
	)
	, index_buffer(
		allocator,
		static_cast<size_t>(
			(this->static_arena_length + this->dynamic_arena_total_length) *
			sizeof(uint32_t)
		),
		vrender::render::memory::BufferUsageClass::INDEX,
		vrender::render::memory::BufferCPUAccess::WRITE_OFTEN,
		vrender::render::memory::BufferLifetime::PERSISTENT
	)
	, static_vertex_suballocator(
		vrender::render::memory::SuballocatorStrategy::FREE_LIST,
		0,
		this->static_arena_length * sizeof(vrender::render::Vertex)
	)
	, static_index_suballocator(
		vrender::render::memory::SuballocatorStrategy::FREE_LIST,
		0,
		this->static_arena_length * sizeof(uint32_t)
	)
{
	// Create Dynamic Suballocators
	uint32_t per_dynamic_allocation_size = this->dynamic_arena_total_length / dynamic_divisions;		// int division

	this->dynamic_vertex_suballocators.reserve(dynamic_divisions);
	this->dynamic_index_suballocators.reserve(dynamic_divisions);
	for (uint16_t i = 0; i < dynamic_divisions; i++)
	{
		this->dynamic_vertex_suballocators.emplace_back(
			vrender::render::memory::SuballocatorStrategy::BUMP,
			sizeof(vrender::render::Vertex) * (
				this->static_arena_length + (i * per_dynamic_allocation_size)
			),
			per_dynamic_allocation_size
		);

		this->dynamic_index_suballocators.emplace_back(
			vrender::render::memory::SuballocatorStrategy::BUMP,
			sizeof(uint32_t) * (
				this->static_arena_length + (i * per_dynamic_allocation_size)
			),
			per_dynamic_allocation_size
		);
	}
}
vrender::render::GeometryArena::~GeometryArena()
{
	this->vertex_buffer.~Buffer();
	this->index_buffer.~Buffer();

	this->static_vertex_suballocator.~Suballocator();
	this->static_index_suballocator.~Suballocator();

	this->dynamic_vertex_suballocators.~vector();
	this->dynamic_index_suballocators.~vector();
}

// API Accessibility
const vrender::render::Mesh vrender::render::GeometryArena::create_static_mesh(
	std::vector<vrender::render::Vertex>& vertices,
	std::vector<uint32_t>& indices
)
{
	// Find Allocation Locations
	uint32_t vertex_offset = this->static_vertex_suballocator.allocate(static_cast<uint32_t>(vertices.size() * sizeof(vrender::render::Vertex)));
	uint32_t index_offset = this->static_index_suballocator.allocate(static_cast<uint32_t>(indices.size() * sizeof(uint32_t)));

	// Validate Offsets
	if (vertex_offset == UINT32_MAX || index_offset == UINT32_MAX)
	{
		std::cerr << "ERROR: VRENDER Could not Allocate Memory for Requested Mesh" << std::endl;
		return { UINT32_MAX, UINT32_MAX, UINT32_MAX };
	}

	// Write Data
	this->vertex_buffer.write(
		vertices.data(),
		vertices.size() * sizeof(vrender::render::Vertex),
		vertex_offset
	);
	this->index_buffer.write(
		indices.data(),
		indices.size() * sizeof(uint32_t),
		index_offset
	);

	// Return Mesh
	return vrender::render::Mesh{
		.vertex_offset = vertex_offset,
		.vertex_offset_count = vertex_offset / sizeof(vrender::render::Vertex),
		.vertex_count = static_cast<uint32_t>(vertices.size()),
		.index_offset = index_offset,
		.index_offset_count = index_offset / sizeof(uint32_t),
		.index_count = static_cast<uint32_t>(indices.size())
	};
}
const vrender::render::Mesh vrender::render::GeometryArena::create_dynamic_mesh(
	std::vector<vrender::render::Vertex>& vertices,
	std::vector<uint32_t>& indices,
	uint32_t index
)
{
	// Find Allocation Offsets
	uint32_t vertex_offset = this->dynamic_vertex_suballocators[index].allocate(vertices.size() * sizeof(vrender::render::Vertex));
	uint32_t index_offset = this->dynamic_index_suballocators[index].allocate(indices.size() * sizeof(uint32_t));

	// Validate Offsets
	if (vertex_offset == UINT32_MAX || index_offset == UINT32_MAX)
	{
		std::cerr << "ERROR: VRENDER Could Not Allocate Memory for Requested Mesh" << std::endl;
		return { UINT32_MAX, UINT32_MAX, UINT32_MAX };
	}

	// Write Data
	this->vertex_buffer.write(
		vertices.data(),
		vertices.size() * sizeof(vrender::render::Vertex),
		vertex_offset
	);
	this->index_buffer.write(
		indices.data(),
		indices.size() * sizeof(uint32_t),
		index_offset
	);

	// Return Mesh
	return vrender::render::Mesh{
		.vertex_offset = vertex_offset,
		.vertex_offset_count = vertex_offset / sizeof(vrender::render::Vertex),
		.vertex_count = static_cast<uint32_t>(vertices.size()),
		.index_offset = index_offset,
		.index_offset_count = index_offset / sizeof(uint32_t),
		.index_count = static_cast<uint32_t>(indices.size())
	};
}

void vrender::render::GeometryArena::reset_dynamic(uint32_t index)
{
	if (index > this->dynamic_vertex_suballocators.size())
	{
		throw std::runtime_error("ERROR: VRENDER Was Requested to Reset Invalid Dynamic Region");
	}

	this->dynamic_vertex_suballocators[index].reset();
	this->dynamic_index_suballocators[index].reset();
}

const vrender::render::memory::Buffer& vrender::render::GeometryArena::get_vertex_buffer() const
{
	return this->vertex_buffer;
}
const vrender::render::memory::Buffer& vrender::render::GeometryArena::get_index_buffer() const
{
	return this->index_buffer;
}