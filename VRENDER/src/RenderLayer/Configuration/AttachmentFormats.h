#ifndef RENDER_CONFIG_ATTACHMENT_FORMATS_H
#define RENDER_CONFIG_ATTACHMENT_FORMATS_H

#include <vector>

#include <vulkan/vulkan.h>

namespace vrender::render::config
{
	struct AttachmentFormats
	{
		std::vector<VkFormat> color_formats;
		VkFormat depth_format;
		VkFormat stencil_format;
	};
}

#endif