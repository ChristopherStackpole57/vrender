#ifndef RENDER_BUFFER_DESC_H
#define RENDER_BUFFER_DESC_H

#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>

namespace vrender::render::memory
{
	enum class BufferUsageClass
	{
		VERTEX,
		UNIFORM,
		STAGING
	};
	enum class BufferCPUAccess
	{
		NONE,
		WRITE_ONCE,
		WRITE_OFTEN
	};
	enum class BufferLifetime
	{
		PERSISTENT,
		PER_FRAME,
		TRANSIENT
	};

	struct BufferDesc
	{
		// Description Schema
		size_t size;
		vrender::render::memory::BufferUsageClass usage_class;
		vrender::render::memory::BufferCPUAccess cpu_access;
		vrender::render::memory::BufferLifetime lifetime;
	};
}

#endif