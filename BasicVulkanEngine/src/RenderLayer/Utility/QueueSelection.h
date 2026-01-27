#ifndef RENDER_UTILITY_QUEUE_SELECTION_H
#define RENDER_UTILITY_QUEUE_SELECTION_H

#include <vulkan/vulkan.h>

#include <RenderLayer/PhysicalDevice.h>

namespace vrender::render
{
	namespace utility::queue
	{
		struct QueueSelection
		{
			uint32_t graphics_family;
			uint32_t present_family;
			uint32_t compute_family;
		};

		QueueSelection select_queue_families(const vrender::render::PhysicalDevice& physical_device, const VkSurfaceKHR& surface);
	}
}

#endif