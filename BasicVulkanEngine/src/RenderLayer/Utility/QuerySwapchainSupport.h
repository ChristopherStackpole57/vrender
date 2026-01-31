#ifndef RENDER_UTILITY_SWAPCHAIN_SUPPORT_H
#define RENDER_UTILITY_SWAPCHAIN_SUPPORT_H

#include <stdexcept>

#include <vulkan/vulkan.h>

#include <PhysicalDevice.h>

namespace vrender::render
{
	namespace utility::swapchain
	{
		struct SwapchainSupport
		{
			VkSurfaceCapabilitiesKHR surface_capabilities;
			std::vector<VkSurfaceFormatKHR> surface_formats;
			std::vector<VkPresentModeKHR> present_modes;
		};

		const vrender::render::utility::swapchain::SwapchainSupport query_swapchain_support(
			const vrender::render::PhysicalDevice& physical_device,
			const VkSurfaceKHR surface
		);
	}
}

#endif