#include "ConfigureSwapchainCreateInfo.h"

// Constants
const VkCompositeAlphaFlagBitsKHR PREFERRED_COMPOSITE_FLAGS[] = {
	VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
	VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
	VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
	VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
};

// Internal Data Structures
struct SharingInfo
{
	VkSharingMode sharing_mode;
	uint32_t queue_family_index_count;
	std::vector<uint32_t> queue_family_indices;
};

// Utility
static uint32_t pick_min_image(const VkSurfaceCapabilitiesKHR surface_capabilities)
{
	uint32_t desired_count = surface_capabilities.minImageCount + 1;
	if (surface_capabilities.maxImageCount > 0)
	{
		return std::min(desired_count, surface_capabilities.maxImageCount);
	}
	return desired_count;
}
static VkSurfaceFormatKHR find_target_format(const std::vector<VkSurfaceFormatKHR>& options)
{
	// Account for undefined format
	if (options.size() == 1 && options[0].format == VK_FORMAT_UNDEFINED)
	{
		return {
			VK_FORMAT_B8G8R8A8_SRGB,
			VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
		};
	}

	// This is the preferred format
	for (const VkSurfaceFormatKHR format : options)
	{
		if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return format;
		}
	}

	// Fallback
	return options[0];
}
static VkExtent2D pick_image_extent(const VkSurfaceCapabilitiesKHR surface_capabilities, const std::pair<int, int> framebuffer_size)
{
	if (surface_capabilities.currentExtent.width != UINT32_MAX)
	{
		return surface_capabilities.currentExtent;
	}

	VkExtent2D extent{};
	extent.width = std::clamp(
		static_cast<uint32_t>(framebuffer_size.first),
		surface_capabilities.minImageExtent.width,
		surface_capabilities.maxImageExtent.width
	);
	extent.height = std::clamp(
		static_cast<uint32_t>(framebuffer_size.second),
		surface_capabilities.minImageExtent.height,
		surface_capabilities.maxImageExtent.height
	);

	return extent;
}
static SharingInfo pick_sharing_info(const vrender::render::utility::queue::QueueSelection selection_result)
{
	if (selection_result.graphics_family == selection_result.present_family)
	{
		return {
			VK_SHARING_MODE_EXCLUSIVE,
			0,
			{}
		};
	}

	return {
		VK_SHARING_MODE_CONCURRENT,
		2,
		{ selection_result.graphics_family, selection_result.present_family }
	};
}
static VkSurfaceTransformFlagBitsKHR pick_pre_transform(const VkSurfaceCapabilitiesKHR surface_capabilities)
{
	if (surface_capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
	{
		return VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	return surface_capabilities.currentTransform;
}
static VkCompositeAlphaFlagBitsKHR pick_composite_alpha(const VkSurfaceCapabilitiesKHR surface_capabilities)
{
	for (const VkCompositeAlphaFlagBitsKHR flag : PREFERRED_COMPOSITE_FLAGS)
	{
		if (surface_capabilities.supportedCompositeAlpha & flag)
		{
			return flag;
		}
	}

	return VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
}
static VkPresentModeKHR pick_present_mode(const std::vector<VkPresentModeKHR> present_modes)
{
	// Look for preference: mailbox mode
	for (VkPresentModeKHR mode : present_modes)
	{
		if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return mode;
		}
	}

	// Look for second preference: imeediate mode
	for (VkPresentModeKHR mode : present_modes)
	{
		if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
		{
			return mode;
		}
	}

	// Fallback
	return VK_PRESENT_MODE_FIFO_KHR;
}

// Configure Swapchain
const vrender::render::utility::swapchain::SwapchainConfiguration vrender::render::utility::swapchain::configure_swapchain(
	const vrender::render::utility::swapchain::SwapchainSupport swapchain_support,
	const vrender::render::utility::queue::QueueSelection queue_selection,
	const std::pair<int, int> framebuffer_size,
	const VkSwapchainKHR old_swapchain
)
{
	vrender::render::utility::swapchain::SwapchainConfiguration configuration;
	configuration.min_image_count = pick_min_image(swapchain_support.surface_capabilities);

	VkSurfaceFormatKHR surface_format = find_target_format(swapchain_support.surface_formats);
	configuration.image_format = surface_format.format;
	configuration.image_color_space = surface_format.colorSpace;

	configuration.image_extent = pick_image_extent(swapchain_support.surface_capabilities, framebuffer_size);

	SharingInfo sharing_info = pick_sharing_info(queue_selection);
	configuration.image_sharing_mode = sharing_info.sharing_mode;
	configuration.queue_family_index_count = sharing_info.queue_family_index_count;
	configuration.queue_family_indices = sharing_info.queue_family_indices;

	configuration.pre_transform = pick_pre_transform(swapchain_support.surface_capabilities);
	configuration.composite_alpha = pick_composite_alpha(swapchain_support.surface_capabilities);
	configuration.present_mode = pick_present_mode(swapchain_support.present_modes);

	configuration.old_swapchain = old_swapchain;

	return configuration;
}