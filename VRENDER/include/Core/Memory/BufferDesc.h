#ifndef RENDER_BUFFER_DESC_H
#define RENDER_BUFFER_DESC_H

#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>

namespace vrender::render::memory
{
	enum class BufferUsageClass
	{
		NONE = 0,
		VERTEX =	1 << 0,
		INDEX =		1 << 1,
		UNIFORM =	1 << 2,
		STAGING =	1 << 3,
		TRANSFER =	1 << 4
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

	inline BufferUsageClass operator|(BufferUsageClass first, BufferUsageClass second)
	{
		return static_cast<BufferUsageClass>(
			static_cast<uint32_t>(first) | static_cast<uint32_t>(second)
		);
	}
	inline BufferUsageClass operator&(BufferUsageClass first, BufferUsageClass second)
	{
		return static_cast<BufferUsageClass>(
			static_cast<uint32_t>(first) & static_cast<uint32_t>(second)
		);
	}
	inline bool has_flag(BufferUsageClass value, BufferUsageClass flag)
	{
		return (value & flag) != BufferUsageClass::NONE;
	}

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