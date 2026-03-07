#include <RenderLayer/Core/GeometryArena.h>

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
const vrender::render::MeshToken vrender::render::GeometryArena::create_static_mesh(
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
		return UINT64_MAX;
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
	vrender::render::Mesh mesh = vrender::render::Mesh{
		.vertex_offset = vertex_offset,
		.vertex_offset_count = vertex_offset / sizeof(vrender::render::Vertex),
		.vertex_count = static_cast<uint32_t>(vertices.size()),
		.index_offset = index_offset,
		.index_offset_count = index_offset / sizeof(uint32_t),
		.index_count = static_cast<uint32_t>(indices.size())
	};

	vrender::render::MeshEntry entry{ mesh };
	return acquire_slot_token(entry);
}
const vrender::render::MeshToken vrender::render::GeometryArena::create_dynamic_mesh(
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
		return UINT64_MAX;
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
	vrender::render::Mesh mesh = vrender::render::Mesh{
		.vertex_offset = vertex_offset,
		.vertex_offset_count = vertex_offset / sizeof(vrender::render::Vertex),
		.vertex_count = static_cast<uint32_t>(vertices.size()),
		.index_offset = index_offset,
		.index_offset_count = index_offset / sizeof(uint32_t),
		.index_count = static_cast<uint32_t>(indices.size())
	};
	vrender::render::MeshEntry entry = { mesh };

	return acquire_slot_token(entry);
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

// Token Arena
const vrender::render::Mesh vrender::render::GeometryArena::get_mesh(vrender::render::MeshToken token) const
{
	if (this->token_valid(token) && this->token_alive(token))
	{
		vrender::render::MeshTokenComponents components = this->decode_token(token);
		return this->slots[components.index].entry.mesh;
	}
}

vrender::render::MeshToken vrender::render::GeometryArena::encode_token(uint64_t index, uint64_t generation) const
{
	return (index << 32) | generation;
}
vrender::render::MeshTokenComponents vrender::render::GeometryArena::decode_token(vrender::render::MeshToken token) const
{
	return {
		static_cast<uint32_t>(token >> 32),
		static_cast<uint32_t>(token)
	};
}
vrender::render::MeshToken vrender::render::GeometryArena::acquire_slot_token(vrender::render::MeshEntry entry)
{
	uint32_t index;
	if (this->free_indices.size() == 0)
	{
		// Must Generate a New Slot
		this->slots.push_back({
			entry,
			0,
			true
			});

		index = static_cast<uint32_t>(this->slots.size() - 1);
	}
	else
	{
		index = this->free_indices.back();
		this->free_indices.pop_back();

		this->slots[index].entry = entry;
		this->slots[index].generation++;
		this->slots[index].alive = true;
	}

	uint32_t generation = this->slots[index].generation;

	return encode_token(index, generation);
}
vrender::render::MeshSlot& vrender::render::GeometryArena::slot_from_token(vrender::render::MeshToken token)
{
	vrender::render::MeshTokenComponents comps = decode_token(token);
	return this->slots[comps.index];
}

bool vrender::render::GeometryArena::token_valid(vrender::render::MeshToken token) const
{
	vrender::render::MeshTokenComponents comps = this->decode_token(token);
	return comps.index < this->slots.size();
}
bool vrender::render::GeometryArena::token_alive(vrender::render::MeshToken token) const 
{
	vrender::render::MeshTokenComponents comps = this->decode_token(token);
	const vrender::render::MeshSlot& slot = this->slots[comps.index];

	return
		slot.alive &&
		slot.generation == comps.generation;
}