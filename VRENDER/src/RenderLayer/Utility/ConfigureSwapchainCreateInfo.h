#ifndef RENDER_UTILITY_CONFIGURE_SWAPCHAIN_CREATE_H
#define RENDER_UTILITY_CONFIGURE_SWAPCHAIN_CREATE_H

#include <algorithm>
#include <vector>

#include <vulkan/vulkan.h>

#include <RenderLayer/Utility/QuerySwapchainSupport.h>
#include <RenderLayer/Utility/QueueSelection.h>

namespace vrender::render
{
	namespace utility::swapchain
	{
		struct SwapchainConfiguration
		{
			uint32_t min_image_count = -1;
			VkFormat image_format{};
			VkColorSpaceKHR image_color_space{};
			VkExtent2D image_extent{};
			uint32_t image_array_layers = 1;
			VkImageUsageFlags image_usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			VkSharingMode image_sharing_mode{};
			uint32_t queue_family_index_count = 0;
			std::vector<uint32_t> queue_family_indices{};
			VkSurfaceTransformFlagBitsKHR pre_transform{};
			VkCompositeAlphaFlagBitsKHR composite_alpha{};
			VkPresentModeKHR present_mode{};
			VkBool32 clipped = VK_TRUE;
			VkSwapchainKHR old_swapchain{};
		};

		const vrender::render::utility::swapchain::SwapchainConfiguration configure_swapchain(
			const vrender::render::utility::swapchain::SwapchainSupport swapchain_support,
			const vrender::render::utility::queue::QueueSelection queue_selection,
			const std::pair<int, int> framebuffer_size,
			const VkSwapchainKHR old_swapchain = VK_NULL_HANDLE
		);
	}
}

#endif