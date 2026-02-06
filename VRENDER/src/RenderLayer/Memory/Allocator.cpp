#include <Core/Memory/Allocator.h>

// Utility
struct PlacementInfo
{
	VmaAllocationCreateFlags flags;
	VmaMemoryUsage usage;
};

static vrender::render::memory::BufferValidationResult buffer_desc_valid(const vrender::render::memory::BufferDesc& desc)
{
	vrender::render::memory::BufferValidationResult validation_result{};
	validation_result.success = true;
	validation_result.error = vrender::render::memory::BufferValidationError::NONE;

	// Validate Components
	//		Ensure provided size is valid
	if (desc.size == 0)
	{
		validation_result.success = false;
		validation_result.error = vrender::render::memory::BufferValidationError::ZERO_SIZE;
		return validation_result;
	}
	//		TODO: Other validation requirements

	// Return Result
	return validation_result;
}
static VkBufferUsageFlags buffer_usage_flag_from_desc_enum(vrender::render::memory::BufferUsageClass usage_class)
{
	switch (usage_class)
	{
	case vrender::render::memory::BufferUsageClass::VERTEX:
		return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	case vrender::render::memory::BufferUsageClass::UNIFORM:
		return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	case vrender::render::memory::BufferUsageClass::STAGING:
		return VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	default:
		// Take advantage of C++ lazy evalation
		assert(false && "ERROR: Invalid BufferUsageClass");
		return 0;
	}
}
static PlacementInfo choose_placement(
	vrender::render::memory::BufferCPUAccess cpu_access
)
{
	switch (cpu_access)
	{
	case vrender::render::memory::BufferCPUAccess::NONE:
		return { 0, VMA_MEMORY_USAGE_GPU_ONLY };
	case vrender::render::memory::BufferCPUAccess::WRITE_ONCE:
		return {
			VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
			VMA_MEMORY_USAGE_AUTO
		};
	case vrender::render::memory::BufferCPUAccess::WRITE_OFTEN:
		return {
			VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT |
			VMA_ALLOCATION_CREATE_MAPPED_BIT,
			VMA_MEMORY_USAGE_AUTO
		};
	}

	throw std::runtime_error("ERROR: Buffer Placement Strategy Could Not Be Chosen; CPU Access State Not Recognized");
}

// Lifetime Control
vrender::render::memory::Allocator::Allocator(
	const vrender::render::Instance& instance,
	const vrender::render::PhysicalDevice& physical_device,
	const vrender::render::LogicalDevice& logical_device
)
	: logical_device_ptr(&logical_device)
{
	VmaAllocatorCreateInfo create_info{};
	create_info.instance = instance.get_instance();
	create_info.physicalDevice = physical_device.get_physical_device();
	create_info.device = logical_device.get_logical_device();
	create_info.flags = 0;
	create_info.pAllocationCallbacks = nullptr;

	VkResult creation_result = vmaCreateAllocator(
		&create_info,
		&this->allocator
	);
	if (creation_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Unable to Create VMA Allocator");
	}
}
vrender::render::memory::Allocator::~Allocator()
{

}

// API Accessibility
vrender::render::memory::BufferAllocationResult vrender::render::memory::Allocator::allocate_buffer(const vrender::render::memory::BufferDesc& desc)
{
	// Validate Description
	vrender::render::memory::BufferValidationResult validation_result = buffer_desc_valid(desc);
	if (validation_result.success == false)
	{
#if DEBUG
		throw std::runtime_error("ERROR: Allocator Recieved Invalid Buffer Description\n\tError: " + 
			std::to_string(static_cast<int>(validation_result.error))
		);
#else
		std::cerr << "ERROR: Allocator Recieved Invalid Buffer Description\n\tError: " << 
			std::to_string(static_cast<int>(validation_result.error)) << std::endl;

		return {};
#endif
	}

	vrender::render::memory::BufferAllocationResult alloc_result{};
	vrender::render::memory::AllocationEntry entry{};

	entry.cpu_access = desc.cpu_access;
	entry.lifetime = desc.lifetime;

	// Create Allocation Info from Description
	VkBufferUsageFlags buffer_flags = buffer_usage_flag_from_desc_enum(desc.usage_class);
	PlacementInfo placement_info = choose_placement(desc.cpu_access);

	// Build Allocation Request
	VmaAllocationCreateInfo alloc_info{};
	alloc_info.flags = placement_info.flags;
	alloc_info.usage = placement_info.usage;

	VkBufferCreateInfo buffer_info{};
	buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_info.pNext = nullptr;
	buffer_info.size = desc.size;
	buffer_info.usage = buffer_flags;
	buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	// Create Allocator and Buffer
	VkResult creation_result = vmaCreateBuffer(
		this->allocator,
		&buffer_info,
		&alloc_info,
		&entry.buffer,
		&entry.allocation,
		&entry.info
	);
	if (creation_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: VMA Unable to Create Buffer");
	}

	alloc_result.buffer = entry.buffer;
	alloc_result.token = this->acquire_slot_token(entry);

	return alloc_result;
}
void vrender::render::memory::Allocator::free_buffer(vrender::render::memory::AllocationToken token)
{
	if (!this->token_is_valid(token) || !this->token_is_alive(token))
	{
		std::cerr << "WARNING: Unable to Process Buffer Deletion Because ";
		if (!this->token_is_valid(token))
		{
			std::cerr << "Provided Token is Invald" << std::endl;
			return;
		}
		std::cerr << "Buffer Has Already Been Deleted" << std::endl;
		return;
	}
	AllocationSlot& slot = this->slots[
		this->decode_token(token).index
	];

	// Delete Buffer...
	vmaDestroyBuffer(
		this->allocator,
		slot.entry.buffer,
		slot.entry.allocation
	);

	// Update Slot
	slot.entry = {};
	slot.generation++;
	slot.alive = false;
}
void* vrender::render::memory::Allocator::map_buffer(vrender::render::memory::AllocationToken token)
{
	if (!token_is_valid(token) || !token_is_alive(token))
	{
		throw std::runtime_error("ERROR: Requested Mapping of Invalid Buffer");
	}

	vrender::render::memory::AllocationSlot& slot = this->slot_from_token(token);
	if (slot.entry.cpu_access == vrender::render::memory::BufferCPUAccess::NONE)
	{
		throw std::runtime_error("ERROR: Attempted to Map GPU Only Buffer");
	}

	void* data = nullptr;
	vmaMapMemory(
		this->allocator,
		slot.entry.allocation,
		&data
	);

	return data;
}
void vrender::render::memory::Allocator::unmap_buffer(vrender::render::memory::AllocationToken token)
{
	if (!token_is_valid(token) || !token_is_alive(token))
	{
		throw std::runtime_error("ERROR: Requested Unmapping of Invalid Buffer");
	}

	vrender::render::memory::AllocationSlot& slot = slot_from_token(token);
	vmaUnmapMemory(this->allocator, slot.entry.allocation);
}
void vrender::render::memory::Allocator::write_buffer(
	vrender::render::memory::AllocationToken token,
	const void* src,
	size_t size,
	size_t offset
)
{
	if (!token_is_valid(token) || !token_is_alive(token))
	{
		throw std::runtime_error("ERROR: Requested Writing of Invalid Buffer");
	}

	vrender::render::memory::AllocationSlot& slot = slot_from_token(token);
	if (slot.entry.cpu_access == vrender::render::memory::BufferCPUAccess::NONE)
	{
		throw std::runtime_error("ERROR: Attempt to Write GPU Only Buffer");
	}

	// TODO: Skip mapping on persistently mapped allocations
	// TODO: validate alignment
	uint8_t* dst = nullptr;
	vmaMapMemory(
		this->allocator,
		slot.entry.allocation,
		reinterpret_cast<void**>(&dst)
	);
	std::memcpy(dst + offset, src, size);
	vmaUnmapMemory(this->allocator, slot.entry.allocation);
}

// Utility
vrender::render::memory::AllocationToken vrender::render::memory::Allocator::encode_token(
	uint64_t index,
	uint64_t generation
)
{
	return (index << 32) | generation;
}
vrender::render::memory::AllocationTokenComponents vrender::render::memory::Allocator::decode_token(
	vrender::render::memory::AllocationToken token
)
{
	return {
		static_cast<uint32_t>(token >> 32),
		static_cast<uint32_t>(token)
	};
}
vrender::render::memory::AllocationToken vrender::render::memory::Allocator::acquire_slot_token(
	vrender::render::memory::AllocationEntry entry
)
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
vrender::render::memory::AllocationSlot& vrender::render::memory::Allocator::slot_from_token(
	vrender::render::memory::AllocationToken token
)
{
	vrender::render::memory::AllocationTokenComponents comps = decode_token(token);
	return this->slots[comps.index];
}

bool vrender::render::memory::Allocator::token_is_valid(vrender::render::memory::AllocationToken token)
{
	vrender::render::memory::AllocationTokenComponents comps = this->decode_token(token);
	return
		comps.index < this->slots.size();
}
bool vrender::render::memory::Allocator::token_is_alive(vrender::render::memory::AllocationToken token)
{
	vrender::render::memory::AllocationTokenComponents comps = this->decode_token(token);
	AllocationSlot& slot = this->slots[comps.index];

	return 
		slot.alive &&
		slot.generation == comps.generation;
}