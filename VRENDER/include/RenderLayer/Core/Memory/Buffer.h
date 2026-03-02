#ifndef RENDER_BUFFER_H
#define RENDER_BUFFER_H

#include <RenderLayer/Core/Memory/Allocator.h>
#include <RenderLayer/Core/Memory/BufferDesc.h>

namespace vrender::render::memory
{
	class Buffer
	{
	public:
		// Lifetime Control
		Buffer(
			vrender::render::memory::Allocator& allocator,
			const size_t size,
			const vrender::render::memory::BufferUsageClass usage_class,
			const vrender::render::memory::BufferCPUAccess cpu_access,
			const vrender::render::memory::BufferLifetime lifetime
		);
		~Buffer();

		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;

		Buffer(Buffer&& other) noexcept;
		Buffer& operator=(Buffer&& other) noexcept;

		// API Accessibility
		// TODO: Support debug names
		VkBuffer get_buffer() const;
		uint32_t get_size() const;

		void write(
			const void* data,
			size_t size,
			size_t offset = 0
		);
	private:
		VkBuffer buffer;
		vrender::render::memory::BufferDesc desc;
		vrender::render::memory::AllocationToken token;

		vrender::render::memory::Allocator* allocator_ptr;
	};
}

#endif