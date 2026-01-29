#ifndef RENDER_SWAPCHAIN_H
#define RENDER_SWAPCHAIN_H

#include <algorithm>

#include <vulkan/vulkan.h>

#include <RenderLayer/LogicalDevice.h>

#include <RenderLayer/Utility/ConfigureSwapchainCreateInfo.h>
#include <RenderLayer/Utility/QuerySwapchainSupport.h>

namespace vrender::render
{
	class Swapchain
	{
	public:
		// Lifetime Control
		Swapchain(
			const vrender::render::LogicalDevice& logical_device,
			const VkSurfaceKHR surface,
			const vrender::render::utility::swapchain::SwapchainConfiguration configuration
		);
		~Swapchain();

		// API Accessibility
	private:
		VkSwapchainKHR swapchain = VK_NULL_HANDLE;
		const vrender::render::LogicalDevice* device_ptr = nullptr;
	};
}

#endif