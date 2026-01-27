#ifndef RENDER_UTILITY_QUEUE_HANDLE_H
#define RENDER_UTILITY_QUEUE_HANDLE_H

#include <vulkan/vulkan.h>

namespace vrender::render
{
	namespace utility::queue
	{
		struct QueueHandle
		{
			VkQueue queue;
			uint32_t family_index;
			uint32_t index;
			VkQueueFlags capabilities;

			// Helpers
			bool supports_graphics()
			{
				return capabilities & VK_QUEUE_GRAPHICS_BIT;
			}
			bool supports_compute()
			{
				return capabilities & VK_QUEUE_COMPUTE_BIT;
			}
			bool supports_transfer_only()
			{
				return (
					capabilities & VK_QUEUE_TRANSFER_BIT &&
					!(capabilities & VK_QUEUE_GRAPHICS_BIT) &&
					!(capabilities & VK_QUEUE_COMPUTE_BIT)
				);
			}
		};
	}
}

#endif