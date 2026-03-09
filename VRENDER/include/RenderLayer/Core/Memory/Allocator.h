#ifndef RENDER_ALLOCATOR_H
#define RENDER_ALLOCATOR_H

#include <iostream>
#include <string>
#include <vector>

#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>

#include <RenderLayer/Core/Instance.h>
#include <RenderLayer/Core/LogicalDevice.h>
#include <RenderLayer/Core/PhysicalDevice.h>

#include <RenderLayer/Core/Memory/CPUAccess.h>
#include <RenderLayer/Core/Memory/BufferDesc.h>
#include <RenderLayer/Core/Memory/ImageDesc.h>

#include <Utility/Generator.h>

// Quick note to add to docs:
/*
The whole process here is to take real-time data *intent* and run it through a policy engine that
provides data fitting the intended use; policy is specified *exclusively* in the Allocator (and any provided
configurations once those are handled later)
*/

namespace vrender::render::memory
{
	// Buffer Allocation
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
	struct BufferAllocationResult
	{
		VkBuffer buffer;
		AllocationHandle handle;
	};
	struct BufferAllocationEntry
	{
		VmaAllocation allocation;
		VmaAllocationInfo info;

		VkBuffer buffer;
		VkDeviceSize size;

		vrender::render::memory::CPUAccess cpu_access;
		vrender::render::memory::BufferLifetime lifetime;
		bool persistently_mapped;
	};



	// Image Allocation
	enum class ImageValidationError
	{

	};
	struct ImageValidationResult
	{
		bool success;
		vrender::render::memory::ImageValidationError error;
	};
	struct ImageAllocationResult
	{
		VkImage image;
		AllocationHandle handle;
	};
	struct ImageAllocationEntry
	{
		VmaAllocation allocation;
		VmaAllocationInfo info;

		VkImage image;
		VkDeviceSize size;

		vrender::render::memory::CPUAccess cpu_access;
	};
	


	// Generic Allocation
	typedef vrender::utility::Handle AllocationHandle;

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
		void free_buffer(AllocationHandle handle);
		void* map_buffer(AllocationHandle handle);
		void unmap_buffer(AllocationHandle handle);
		void write_buffer(
			AllocationHandle handle,
			const void* src,
			size_t size,
			size_t offset
		);

		vrender::render::memory::ImageAllocationResult allocate_image(const vrender::render::memory::ImageDesc& image);
		void free_image(AllocationHandle handle);
	private:
		VmaAllocator allocator;
		const vrender::render::LogicalDevice* logical_device_ptr;
		
		vrender::utility::Generator<BufferAllocationEntry> buffer_generator;
		vrender::utility::Generator<ImageAllocationEntry> image_generator;
	};
}

#endif