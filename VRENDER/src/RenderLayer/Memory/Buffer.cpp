#include <RenderLayer/Core/Memory/Buffer.h>

// Lifetime Control
vrender::render::memory::Buffer::Buffer(
	vrender::render::memory::Allocator& allocator,
	const size_t size,
	const vrender::render::memory::BufferUsageClass usage_class,
	const vrender::render::memory::CPUAccess cpu_access,
	const vrender::render::memory::BufferLifetime lifetime
)
	: allocator_ptr(&allocator)
{
	// Validate Intent
	// TODO: Validate

	// Create BufferDesc
	vrender::render::memory::BufferDesc desc{ size, usage_class, cpu_access, lifetime };
	this->desc = desc;

	// Request Buffer Allocation from Allocator
	vrender::render::memory::BufferAllocationResult alloc_result = allocator.allocate_buffer(desc);
	//TODO: Validate
	this->buffer = alloc_result.buffer;
	this->handle = alloc_result.handle;
}
vrender::render::memory::Buffer::~Buffer()
{
	if (this->handle == vrender::utility::NULL_HANDLE)
	{
		return;
	}

	this->allocator_ptr->free_buffer(this->handle);
	this->handle = vrender::utility::NULL_HANDLE;
}

// NOTE: Buffers do NOT own their VkBuffer, so they MUST NOT destroy it
vrender::render::memory::Buffer::Buffer(vrender::render::memory::Buffer&& other) noexcept
	: buffer(other.buffer)
	, desc(other.desc)
	, handle(other.handle)
	, allocator_ptr(other.allocator_ptr)
{
	other.buffer = VK_NULL_HANDLE;
	other.handle = vrender::utility::NULL_HANDLE;
}
vrender::render::memory::Buffer& vrender::render::memory::Buffer::operator=(
	vrender::render::memory::Buffer&& other
) noexcept
{
	if (this != &other)
	{
		this->buffer = other.buffer;
		this->desc = other.desc;
		this->handle = other.handle;
		this->allocator_ptr = other.allocator_ptr;

		other.buffer = VK_NULL_HANDLE;
		other.handle = vrender::utility::NULL_HANDLE;
	}

	return *this;
}

// API Accessibility
VkBuffer vrender::render::memory::Buffer::get_buffer() const
{
	return this->buffer;
}
uint32_t vrender::render::memory::Buffer::get_size() const
{
	return this->desc.size;
}

void vrender::render::memory::Buffer::write(
	void const* data,
	size_t size,
	size_t offset
)
{
	// Validate Data
	if (offset + size > this->desc.size)
	{
		throw std::runtime_error("ERROR: Tried to Write Past Bufer Size Limit");
	}
	if (this->desc.cpu_access == vrender::render::memory::CPUAccess::NONE)
	{
		throw std::runtime_error("ERROR: This Buffer Does Not Allow Writing");
	}

	// Write Data
	this->allocator_ptr->write_buffer(
		this->handle,
		data,
		size,
		offset
	);
}