#ifndef RENDER_CONFIG_FRAME_DESCRIPTION_H
#define RENDER_CONFIG_FRAME_DESCRIPTION_H

#include <vulkan/vulkan.h>

namespace vrender::render::config
{
	struct FrameDescription
	{
		VkClearColorValue clear_color = { 0.1f, 0.1f, 0.1f, 1.0f };
		VkImage swapchain_image;
		VkImageView swapchain_image_view;
		VkExtent2D extent;
	};
}

#endif