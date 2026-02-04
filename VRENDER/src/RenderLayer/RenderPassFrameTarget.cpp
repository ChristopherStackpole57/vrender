#include <LegacyRender/RenderPassFrameTarget.h>

// Lifetime Control
vrender::render::RenderPassFrameTarget::RenderPassFrameTarget(
	const vrender::render::Framebuffer& framebuffer,
	const vrender::render::RenderPass& render_pass
)
	: framebuffer(framebuffer)
	, render_pass(render_pass)
{

}

// Interface Methods
void vrender::render::RenderPassFrameTarget::begin(VkCommandBuffer command_buffer) const
{
	// TODO: Validate cmd buffer

	VkRenderPassBeginInfo begin_info{};
	begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	begin_info.pNext = nullptr;
	begin_info.renderPass = this->render_pass.get_render_pass();
	begin_info.framebuffer = this->framebuffer.get_framebuffer();
	begin_info.renderArea = {
		.offset = { 0, 0 },
		.extent = this->framebuffer.get_extent()
	};
	begin_info.clearValueCount = this->render_pass.get_attachment_count();

	// Create Clear Values
	// TODO: Extend this to generic render pass
	VkClearValue clear_color{};
	clear_color.color = { { 0.19f, 0.16f, 0.411f, 1.0f } };

	std::vector<VkClearValue> clear_values;
	clear_values.push_back(clear_color);

	begin_info.pClearValues = clear_values.data();

	vkCmdBeginRenderPass(
		command_buffer,
		&begin_info,
		VK_SUBPASS_CONTENTS_INLINE
	);
}
void vrender::render::RenderPassFrameTarget::end(VkCommandBuffer command_buffer) const
{
	// TODO: Validate cmd buffer

	vkCmdEndRenderPass(command_buffer);
}
const VkExtent2D vrender::render::RenderPassFrameTarget::get_extent() const
{
	return this->framebuffer.get_extent();
}