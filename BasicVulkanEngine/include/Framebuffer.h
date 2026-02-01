#ifndef RENDER_FRAMEBUFFER_H
#define RENDER_FRAMEBUFFER_H

#include <vulkan/vulkan.h>

#include <LogicalDevice.h>
#include <RenderPass.h>

namespace vrender::render
{
	class Framebuffer
	{
	public:
		// Lifetime Control
		Framebuffer(
			const vrender::render::LogicalDevice& logical_device,
			const vrender::render::RenderPass& render_pass,
			const std::vector<VkImageView>& attachments,
			const VkExtent2D extent,
			uint32_t layers = 1
		);
		~Framebuffer();

		Framebuffer(const Framebuffer&) = delete;
		Framebuffer& operator=(const Framebuffer&) = delete;

		Framebuffer(Framebuffer&& other) noexcept;
		Framebuffer& operator=(Framebuffer&& other) noexcept;

		// API Accessibility
		const VkFramebuffer get_framebuffer() const;
		const VkExtent2D get_extent() const;
	private:
		VkFramebuffer framebuffer;
		VkExtent2D extent;
		const vrender::render::LogicalDevice* logical_device_ptr;
	};
}

#endif