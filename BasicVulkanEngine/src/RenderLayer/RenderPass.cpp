#include "RenderPass.h"

// Lifetime Control
vrender::render::RenderPass::RenderPass(
	const vrender::render::LogicalDevice& logical_device,
	const vrender::render::config::RenderPassConfig render_pass_config
)
	: logical_device_ptr(&logical_device)
	, config(render_pass_config)
{
	VkRenderPassCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	create_info.pNext = nullptr;
	create_info.flags = 0;
	create_info.attachmentCount = static_cast<uint32_t>(render_pass_config.attachment_descriptions.size());
	create_info.pAttachments = render_pass_config.attachment_descriptions.data();
	create_info.subpassCount = static_cast<uint32_t>(render_pass_config.subpass_descriptions.size());
	create_info.pSubpasses = render_pass_config.subpass_descriptions.data();
	create_info.dependencyCount = static_cast<uint32_t>(render_pass_config.subpass_dependencies.size());
	create_info.pDependencies = render_pass_config.subpass_dependencies.data();

	VkResult creation_result = vkCreateRenderPass(
		logical_device.get_logical_device(),
		&create_info,
		nullptr,
		&this->render_pass
	);
	if (creation_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Failed to Create RenderPass");
	}
}
vrender::render::RenderPass::~RenderPass()
{
	if (this->render_pass == VK_NULL_HANDLE)
	{
		return;
	}

	vkDestroyRenderPass(
		this->logical_device_ptr->get_logical_device(),
		this->render_pass,
		nullptr
	);

	this->render_pass = VK_NULL_HANDLE;
}

// API Accessibility
const VkRenderPass vrender::render::RenderPass::get_render_pass() const
{
	return this->render_pass;
}

uint32_t vrender::render::RenderPass::get_attachment_count() const
{
	return static_cast<uint32_t>(this->config.attachment_descriptions.size());
}
const VkFormat vrender::render::RenderPass::get_attachment_format(uint32_t attachment_index) const
{
	return this->config.attachment_descriptions[attachment_index].format;
}
const VkSampleCountFlagBits vrender::render::RenderPass::get_samples() const
{
	return this->config.attachment_descriptions[0].samples;
}

uint32_t vrender::render::RenderPass::get_subpass_count() const
{
	return static_cast<uint32_t>(this->config.subpass_descriptions.size());
}
bool vrender::render::RenderPass::subpass_has_depth(uint32_t subpass_index) const
{
	return this->config.subpass_descriptions[subpass_index].pDepthStencilAttachment != nullptr;
}
uint32_t vrender::render::RenderPass::get_color_attachment_count(uint32_t subpass_index) const
{
	return static_cast<uint32_t>(this->config.subpass_descriptions[subpass_index].colorAttachmentCount);
}

const VkImageLayout vrender::render::RenderPass::get_initial_layout(uint32_t attachment_index) const
{
	return this->config.attachment_descriptions[attachment_index].initialLayout;
}
const VkImageLayout vrender::render::RenderPass::get_final_layout(uint32_t attachment_index) const
{
	return this->config.attachment_descriptions[attachment_index].finalLayout;
}