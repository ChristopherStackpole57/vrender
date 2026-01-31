#ifndef RENDER_RENDERPASS_H
#define RENDER_RENDERPASS_H

#include <vulkan/vulkan.h>

#include <LogicalDevice.h>

#include <RenderLayer/Configuration/RenderPassConfiguration.h>

namespace vrender::render
{
	class RenderPass
	{
	public:
		// Lifetime Control
		RenderPass(
			const vrender::render::LogicalDevice& logical_device,
			const vrender::render::config::RenderPassConfig render_pass_config
		);
		~RenderPass();

		// API Accessibility
		const VkRenderPass get_render_pass() const;

		uint32_t get_attachment_count() const;
		const VkFormat get_attachment_format(uint32_t attachment_index) const;
		const VkSampleCountFlagBits get_samples() const;

		uint32_t get_subpass_count() const;
		bool subpass_has_depth(uint32_t subpass_index) const;
		uint32_t get_color_attachment_count(uint32_t subpass_index) const;

		const VkImageLayout get_initial_layout(uint32_t attachment_index) const;
		const VkImageLayout get_final_layout(uint32_t attachment_index) const;
	private:
		VkRenderPass render_pass;
		const vrender::render::LogicalDevice* logical_device_ptr;
		const vrender::render::config::RenderPassConfig config;

		uint32_t attachment_count = 0;
	};
}

#endif