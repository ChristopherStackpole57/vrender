#ifndef RENDER_SEMAPHORE_H
#define RENDER_SEMAPHORE_H

#include <vulkan/vulkan.h>

#include <RenderLayer/LogicalDevice.h>

namespace vrender::render
{
	class Semaphore
	{
	public:
		// Lifetime Control
		Semaphore(const vrender::render::LogicalDevice& logical_device);
		~Semaphore();

		// API Accessibility
		const VkSemaphore get_semaphore() const;
	private:
		const vrender::render::LogicalDevice* device_ptr = nullptr;
		VkSemaphore semaphore;
	};
}

#endif