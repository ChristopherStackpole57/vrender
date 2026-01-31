#ifndef RENDER_CONFIG_RENDERPASS_H
#define RENDER_CONFIG_RENDERPASS_H

#include <vulkan/vulkan.h>

namespace vrender::render
{
	namespace config
	{
		struct RenderPassConfig
		{
			std::vector<VkAttachmentDescription> attachment_descriptions;
			std::vector<VkSubpassDescription> subpass_descriptions;
			std::vector<VkSubpassDependency> subpass_dependencies;

			std::vector<VkAttachmentReference> input_attachment_references;
			std::vector<VkAttachmentReference> color_attachment_references;
			std::vector<VkAttachmentReference> resolve_attachment_references;
			std::vector<VkAttachmentReference> preserve_attachment_references;
		};
	}
}

#endif