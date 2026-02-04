#include "QuerySwapchainSupport.h"

const vrender::render::utility::swapchain::SwapchainSupport vrender::render::utility::swapchain::query_swapchain_support(
	const vrender::render::PhysicalDevice& physical_device,
	const VkSurfaceKHR surface
)
{
	// TODO: Add in stronger parameter validation guards

	// Find Surface Capabilities
	vrender::render::utility::swapchain::SwapchainSupport swapchain_support;
	VkResult surface_capabilities_result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
		physical_device.get_physical_device(),
		surface,
		&swapchain_support.surface_capabilities
	);
	if (surface_capabilities_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Unable to Query Surface Capabilities");
	}

	// Find Formats
	uint32_t format_count = 0;
	VkResult format_result = vkGetPhysicalDeviceSurfaceFormatsKHR(
		physical_device.get_physical_device(),
		surface,
		&format_count,
		nullptr
	);
	if (format_result != VK_SUCCESS || format_count == 0)
	{
		throw std::runtime_error("ERROR: Vulkan Was Unable to Find Surface Formats Phase One");
	}

	swapchain_support.surface_formats.resize(format_count);
	format_result = vkGetPhysicalDeviceSurfaceFormatsKHR(
		physical_device.get_physical_device(),
		surface,
		&format_count,
		swapchain_support.surface_formats.data()
	);
	if (format_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Was Unable to Find Surface Formats Phase Two");
	}

	// Find Present Modes
	uint32_t modes_count = 0;
	VkResult mode_results = vkGetPhysicalDeviceSurfacePresentModesKHR(
		physical_device.get_physical_device(),
		surface,
		&modes_count,
		nullptr
	);
	if (mode_results != VK_SUCCESS || modes_count == 0)
	{
		throw std::runtime_error("ERROR: Vulkan Was Unable to Find Surface Present Modes Phase One");
	}

	swapchain_support.present_modes.resize(modes_count);
	mode_results = vkGetPhysicalDeviceSurfacePresentModesKHR(
		physical_device.get_physical_device(),
		surface,
		&modes_count,
		swapchain_support.present_modes.data()
	);
	if (mode_results != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Was Unable to Find Surface Present Modes Phase Two");
	}

	return swapchain_support;
}