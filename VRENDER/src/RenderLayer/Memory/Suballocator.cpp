#include <Core/Memory/Suballocator.h>

// Lifetime Control
vrender::render::memory::Suballocator::Suballocator(
	vrender::render::memory::Buffer* target, 
	uint32_t start, 
	uint32_t size
)
	: target(target)
	, start(start)
	, size(size)
{

}
vrender::render::memory::Suballocator::~Suballocator()
{

}

// API Accessibility
uint32_t vrender::render::memory::Suballocator::allocate(const uint32_t size)
{

}
void vrender::render::memory::Suballocator::free(const vrender::render::Mesh& mesh)
{

}
void vrender::render::memory::Suballocator::reset()
{

}

// Allocation Strategies
uint32_t vrender::render::memory::Suballocator::alloc_bump(const uint32_t size)
{

}
uint32_t vrender::render::memory::Suballocator::alloc_free_list(const uint32_t size)
{

}