#ifndef RENDER_ALLOCATOR_H
#define RENDER_ALLOCATOR_H

#include <iostream>
#include <string>
#include <vector>

#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>

#include <Core/Instance.h>
#include <Core/LogicalDevice.h>
#include <Core/PhysicalDevice.h>
#include <Core/Memory/BufferDesc.h>

// Quick note to add to docs:
/*
The whole process here is to take real-time data *intent* and run it through a policy engine that
provides data fitting the intended use; policy is specified *exclusively* in the Allocator (and any provided
configurations once those are handled later)
*/

namespace vrender::render::memory
{
	enum class BufferValidationError
	{
		NONE,
		ZERO_SIZE,
		INVALID_USAGE_FLAGS,
		MISSING_REQUIRED_FLAGS,
		INCOMPATIBLE_MEMORY_USAGE,
		ALIGNMENT_VIOLATION
	};
	struct BufferValidationResult
	{
		bool success;
		vrender::render::memory::BufferValidationError error;
	};

	using AllocationToken = uint64_t;
	struct BufferAllocationResult
	{
		VkBuffer buffer;
		AllocationToken token;
		// possibly other things
	};

	struct AllocationEntry
	{
		VmaAllocation allocation;
		VmaAllocationInfo info;

		VkBuffer buffer;
		VkDeviceSize size;

		vrender::render::memory::BufferCPUAccess cpu_access;
		vrender::render::memory::BufferLifetime lifetime;
		bool persistently_mapped;

		std::string debug_name;
	};

	struct AllocationSlot
	{
		AllocationEntry entry;
		uint32_t generation;
		bool alive;
	};
	struct AllocationTokenComponents
	{
		uint32_t index;
		uint32_t generation;
	};

	class Allocator
	{
	public:
		// Lifetime Control
		Allocator(
			const vrender::render::Instance& instance,
			const vrender::render::PhysicalDevice& physical_device,
			const vrender::render::LogicalDevice& logical_device
		);
		~Allocator();

		Allocator(const Allocator&) = delete;
		Allocator& operator=(const Allocator&) = delete;

		// API Accessibility
		vrender::render::memory::BufferAllocationResult allocate_buffer(const vrender::render::memory::BufferDesc& desc);
		void free_buffer(AllocationToken token);
		void* map_buffer(AllocationToken token);
		void unmap_buffer(AllocationToken token);
		void write_buffer(
			AllocationToken token,
			const void* src,
			size_t size,
			size_t offset
		);
	private:
		// Utility
		AllocationToken encode_token(uint64_t index, uint64_t generation);
		AllocationTokenComponents decode_token(AllocationToken token);
		AllocationToken acquire_slot_token(AllocationEntry entry);
		AllocationSlot& slot_from_token(AllocationToken token);

		bool token_is_valid(AllocationToken token);
		bool token_is_alive(AllocationToken token);

		VmaAllocator allocator;
		std::vector<AllocationSlot> slots;
		std::vector<uint32_t> free_indices;
		const vrender::render::LogicalDevice* logical_device_ptr;
	};
}

#endif