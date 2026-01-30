#ifndef RENDER_FENCE_H
#define RENDER_FENCE_H

#include <vulkan/vulkan.h>

namespace vrender::render
{
	class Fence
	{
	public:
		// Lifetime Control
		Fence();
		~Fence();

		// API Accessibility
	private:
		VkFence fence = VK_NULL_HANDLE;
	};
}

#endif