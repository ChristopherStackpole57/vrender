#include "Framebuffer.h"

// Lifetime Control
vrender::render::Framebuffer::Framebuffer(
	const vrender::render::LogicalDevice& logical_device,
	const vrender::render::RenderPass& render_pass,
	const std::vector<VkImageView>& attachments,
	const VkExtent2D extent,
	uint32_t layers
)
	: extent(extent)
	, logical_device_ptr(&logical_device)
{
	VkFramebufferCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	create_info.pNext = nullptr;
	create_info.flags = 0;
	create_info.renderPass = render_pass.get_render_pass();
	create_info.attachmentCount = static_cast<uint32_t>(attachments.size());
	create_info.pAttachments = attachments.data();
	create_info.width = extent.width;
	create_info.height = extent.height;
	create_info.layers = layers;

	VkResult creation_result = vkCreateFramebuffer(
		logical_device.get_logical_device(),
		&create_info,
		nullptr,
		&this->framebuffer
	);
}
vrender::render::Framebuffer::~Framebuffer()
{
	if (this->framebuffer == VK_NULL_HANDLE)
	{
		return;
	}

	vkDestroyFramebuffer(
		this->logical_device_ptr->get_logical_device(),
		this->framebuffer,
		nullptr
	);

	this->framebuffer = VK_NULL_HANDLE;
}

// API Accessibility
const VkFramebuffer vrender::render::Framebuffer::get_framebuffer() const
{
	return this->framebuffer;
}
const VkExtent2D vrender::render::Framebuffer::get_extent() const
{
	return this->extent;
}