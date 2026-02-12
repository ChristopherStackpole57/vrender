#ifndef RENDER_SUBALLOCATOR_H
#define RENDER_SUBALLOCATOR_H

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
		Suballocator(vrender::render::memory::Buffer* target, uint32_t start, uint32_t size);
		~Suballocator();

		// API Accessibility
		uint32_t allocate(const uint32_t size);
		void free(const vrender::render::Mesh& mesh);
		void reset();
	private:
		vrender::render::memory::Buffer* target;

		// Core
		uint32_t start;
		uint32_t size;
	
		// Strategy Dependant
		uint32_t head;

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