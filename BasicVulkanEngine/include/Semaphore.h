#ifndef RENDER_SEMAPHORE_H
#define RENDER_SEMAPHORE_H

#include <vulkan/vulkan.h>

#include <LogicalDevice.h>

namespace vrender::render
{
	class Semaphore
	{
	public:
		// Lifetime Control
		Semaphore(const vrender::render::LogicalDevice& logical_device);
		~Semaphore();

		Semaphore(const Semaphore&) = delete;
		Semaphore& operator=(const Semaphore&) = delete;

		Semaphore(Semaphore&& other) noexcept;
		Semaphore& operator=(Semaphore&& other) noexcept;

		// API Accessibility
		const VkSemaphore get_semaphore() const;

		const vrender::render::LogicalDevice* device_ptr = nullptr;
	private:
		
		VkSemaphore semaphore;
	};
}

#endif