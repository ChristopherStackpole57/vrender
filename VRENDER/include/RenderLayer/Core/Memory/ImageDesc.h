#ifndef RENDER_IMAGE_DESC_H
#define RENDER_IMAGE_DESC_H

#include <cstdint>

#include <vulkan/vulkan.h>

#include <RenderLayer/Core/Memory/CPUAccess.h>

namespace vrender::render::memory
{
	struct ImageDesc
	{
		uint32_t width;
		uint32_t height;

		VkFormat format;

		VkImageUsageFlags usage;

		uint32_t mip_levels = 1;
		uint32_t layers = 1;

		VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;

		vrender::render::memory::CPUAccess cpu_access;
	};
}

#endif