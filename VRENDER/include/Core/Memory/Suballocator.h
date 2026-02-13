#ifndef RENDER_SUBALLOCATOR_H
#define RENDER_SUBALLOCATOR_H

#include <algorithm>
#include <iostream>

#include <Core/Mesh.h>

#include <Core/Memory/Allocator.h>
#include <Core/Memory/Buffer.h>

namespace vrender::render::memory
{
	enum class SuballocatorStrategy
	{
		BUMP,
		FREE_LIST,

	};

	class Suballocator
	{
	public:
		// Lifetime Control
		Suballocator(SuballocatorStrategy strategy, vrender::render::memory::Buffer* target, uint32_t start, uint32_t size);
		~Suballocator();

		Suballocator(const Suballocator&) = delete;
		Suballocator& operator=(const Suballocator&) = delete;

		Suballocator(Suballocator&& other) noexcept = default;
		Suballocator& operator=(Suballocator&& other) noexcept = default;

		// API Accessibility
		uint32_t allocate(const uint32_t size);
		void free(const uint32_t offset, const uint32_t size);
		void reset();
	private:
		// Utility
		void defragment_blocks();

		vrender::render::memory::Buffer* target;

		// Core
		SuballocatorStrategy strategy;
		uint32_t region_start;
		uint32_t region_size;
	
		// Strategy Dependant
		uint32_t head = 0;

		struct FreeBlock
		{
			uint32_t offset;
			uint32_t size;
		};
		std::vector<FreeBlock> free_blocks;

		// Allocator Strategy Implementations
		uint32_t alloc_bump(const uint32_t size);
		uint32_t alloc_free_list(const uint32_t size);
	};
}

#endif