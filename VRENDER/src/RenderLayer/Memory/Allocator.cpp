#include <RenderLayer/Core/Memory/Allocator.h>

// Utility
struct PlacementInfo
{
	VmaAllocationCreateFlags flags;
	VmaMemoryUsage usage;
};

//		Buffeer Generation
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
	VkBufferUsageFlags flags = 0;
	
	if (has_flag(usage_class, vrender::render::memory::BufferUsageClass::VERTEX))
		flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

	if (has_flag(usage_class, vrender::render::memory::BufferUsageClass::INDEX))
		flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

	if (has_flag(usage_class, vrender::render::memory::BufferUsageClass::UNIFORM))
		flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

	if (has_flag(usage_class, vrender::render::memory::BufferUsageClass::STAGING))
		flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

	if (has_flag(usage_class, vrender::render::memory::BufferUsageClass::TRANSFER))
		flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;

	if (has_flag(usage_class, vrender::render::memory::BufferUsageClass::STORAGE))
		flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;

	assert(flags != 0 && "ERROR: BufferUsage must not be NONE");

	return flags;
}
static PlacementInfo choose_placement(
	vrender::render::memory::CPUAccess cpu_access
)
{
	switch (cpu_access)
	{
	case vrender::render::memory::CPUAccess::NONE:
		return { 0, VMA_MEMORY_USAGE_GPU_ONLY };
	case vrender::render::memory::CPUAccess::WRITE_ONCE:
		return {
			VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
			VMA_MEMORY_USAGE_AUTO
		};
	case vrender::render::memory::CPUAccess::WRITE_OFTEN:
		return {
			VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT |
			VMA_ALLOCATION_CREATE_MAPPED_BIT,
			VMA_MEMORY_USAGE_AUTO
		};
	}

	throw std::runtime_error("ERROR: Buffer Placement Strategy Could Not Be Chosen; CPU Access State Not Recognized");
}

//		Image Generation
static vrender::render::memory::ImageValidationResult image_desc_valid(const vrender::render::memory::ImageDesc& desc)
{

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
	this->buffer_generator.for_each_living_entry(
		[&](const vrender::render::memory::BufferAllocationEntry& entry)
		{
			vmaDestroyBuffer(
				this->allocator,
				entry.buffer,
				entry.allocation
			);

			vmaFreeMemory(
				this->allocator,
				entry.allocation
			);
		}
	);
}

// API Accessibility
//		Buffer Generation
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
	vrender::render::memory::BufferAllocationEntry entry{};

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
	alloc_result.handle = this->buffer_generator.acquire_slot_handle(entry);

	return alloc_result;
}
void vrender::render::memory::Allocator::free_buffer(vrender::render::memory::AllocationHandle handle)
{
	if (!this->buffer_generator.handle_valid(handle) || !this->buffer_generator.handle_alive(handle))
	{
		std::cerr << "WARNING: Unable to Process Buffer Deletion Because ";
		if (!this->buffer_generator.handle_valid(handle))
		{
			std::cerr << "Provided Handle is Invald" << std::endl;
			return;
		}
		std::cerr << "Buffer Has Already Been Deleted" << std::endl;
		return;
	}
	vrender::render::memory::BufferAllocationEntry& entry = this->buffer_generator.entry_from_handle(handle);

	// Delete Buffer
	vmaDestroyBuffer(
		this->allocator,
		entry.buffer,
		entry.allocation
	);

	// Update Slot
	// TODO: delete entry
	//slot.entry = {};
	//slot.generation++;
	//slot.alive = false;
}
void* vrender::render::memory::Allocator::map_buffer(vrender::render::memory::AllocationHandle handle)
{
	if (!this->buffer_generator.handle_valid(handle) || !this->buffer_generator.handle_alive(handle))
	{
		throw std::runtime_error("ERROR: Requested Mapping of Invalid Buffer");
	}

	vrender::render::memory::BufferAllocationEntry& entry = this->buffer_generator.entry_from_handle(handle);
	if (entry.cpu_access == vrender::render::memory::CPUAccess::NONE)
	{
		throw std::runtime_error("ERROR: Attempted to Map GPU Only Buffer");
	}

	void* data = nullptr;
	vmaMapMemory(
		this->allocator,
		entry.allocation,
		&data
	);

	return data;
}
void vrender::render::memory::Allocator::unmap_buffer(vrender::render::memory::AllocationHandle handle)
{
	if (!this->buffer_generator.handle_valid(handle) || !this->buffer_generator.handle_alive(handle))
	{
		throw std::runtime_error("ERROR: Requested Unmapping of Invalid Buffer");
	}

	vrender::render::memory::BufferAllocationEntry& entry = this->buffer_generator.entry_from_handle(handle);
	vmaUnmapMemory(this->allocator, entry.allocation);
}
void vrender::render::memory::Allocator::write_buffer(
	vrender::render::memory::AllocationHandle handle,
	const void* src,
	size_t size,
	size_t offset
)
{
	if (!this->buffer_generator.handle_valid(handle) || !this->buffer_generator.handle_alive(handle))
	{
		throw std::runtime_error("ERROR: Requested Writing of Invalid Buffer");
	}

	vrender::render::memory::BufferAllocationEntry& entry = this->buffer_generator.entry_from_handle(handle);
	if (entry.cpu_access == vrender::render::memory::CPUAccess::NONE)
	{
		throw std::runtime_error("ERROR: Attempt to Write GPU Only Buffer");
	}

	// TODO: Skip mapping on persistently mapped allocations
	// TODO: validate alignment
	uint8_t* dst = nullptr;
	vmaMapMemory(
		this->allocator,
		entry.allocation,
		reinterpret_cast<void**>(&dst)
	);
	std::memcpy(dst + offset, src, size);
	vmaUnmapMemory(this->allocator, entry.allocation);
}

//		Image Generation
vrender::render::memory::ImageAllocationResult vrender::render::memory::Allocator::allocate_image(const vrender::render::memory::ImageDesc& desc)
{
	// Validate Description
	vrender::render::memory::ImageValidationResult validation_result = image_desc_valid(desc);
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

	vrender::render::memory::ImageAllocationResult alloc_result{};
	vrender::render::memory::ImageAllocationEntry entry{};

	entry.cpu_access = desc.cpu_access;

	// Create Allocation Info from Description
	PlacementInfo placement_info = choose_placement(desc.cpu_access);

	// Build Allocation Request
	VmaAllocationCreateInfo alloc_info{};
	alloc_info.flags = placement_info.flags;
	alloc_info.usage = placement_info.usage;

	VkImageCreateInfo image_info{};
	image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image_info.pNext = nullptr;
	image_info.extent.width = desc.width;
	image_info.extent.height = desc.height;
	image_info.extent.depth = 1;
	image_info.mipLevels = desc.mip_levels;
	image_info.arrayLayers = desc.layers;
	image_info.format = desc.format;
	image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
	image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	image_info.usage = desc.usage;
	image_info.samples = desc.samples;
	image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	// Create Allocator and Buffer
	VkResult creation_result = vmaCreateImage(
		this->allocator,
		&image_info,
		&alloc_info,
		&entry.image,
		&entry.allocation,
		&entry.info
	);
	if (creation_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: VMA Unable to Create Buffer");
	}

	alloc_result.image = entry.image;
	alloc_result.handle = this->image_generator.acquire_slot_handle(entry);

	return alloc_result;
}
void vrender::render::memory::Allocator::free_image(AllocationHandle handle)
{
	if (!this->image_generator.handle_valid(handle) || !this->image_generator.handle_alive(handle))
	{
		std::cerr << "WARNING: Unable to Process Image Deletion Because ";
		if (!this->image_generator.handle_valid(handle))
		{
			std::cerr << "Provided Handle is Invald" << std::endl;
			return;
		}
		std::cerr << "Image Has Already Been Deleted" << std::endl;
		return;
	}
	vrender::render::memory::ImageAllocationEntry& entry = this->image_generator.entry_from_handle(handle);

	// Delete Buffer
	vmaDestroyImage(
		this->allocator,
		entry.image,
		entry.allocation
	);

	// Update Slot
	// TODO: delete entry
	//slot.entry = {};
	//slot.generation++;
	//slot.alive = false;
}