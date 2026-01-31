#ifndef RENDER_UTILITY_QUEUE_SELECTION_H
#define RENDER_UTILITY_QUEUE_SELECTION_H

#include <climits>

#include <vulkan/vulkan.h>

#include <PhysicalDevice.h>

namespace vrender::render
{
	namespace utility::queue
	{
		struct QueueSelection
		{
			uint32_t graphics_family = INT_MAX;
			uint32_t present_family = INT_MAX;
			uint32_t compute_family = INT_MAX;
		};

		QueueSelection select_queue_families(const vrender::render::PhysicalDevice& physical_device, const VkSurfaceKHR& surface);
	}
}

#endif