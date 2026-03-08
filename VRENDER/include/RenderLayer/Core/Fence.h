#ifndef RENDER_FENCE_H
#define RENDER_FENCE_H

#include <vulkan/vulkan.h>

#include <RenderLayer/Core/LogicalDevice.h>

namespace vrender::render
{
	class Fence
	{
	public:
		// Lifetime Control
		Fence(const vrender::render::LogicalDevice& logical_device);
		~Fence();

		Fence(const Fence&) = delete;
		Fence& operator=(const Fence&) = delete;

		Fence(Fence&& other) noexcept;
		Fence& operator=(Fence&& other) noexcept;

		// API Accessibility
		const VkFence get_fence() const;
	private:
		const vrender::render::LogicalDevice* logical_device_ptr;
		VkFence fence = VK_NULL_HANDLE;
	};
}

#endif