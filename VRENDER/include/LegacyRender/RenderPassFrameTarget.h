#ifndef RENDER_RENDER_PASS_FRAME_TARGET_H
#define RENDER_RENDER_PASS_FRAME_TARGET_H

#include <vulkan/vulkan.h>

#include <Core/IFrameTarget.h>

#include <LegacyRender/Framebuffer.h>
#include <LegacyRender/RenderPass.h>

namespace vrender::render
{
	class RenderPassFrameTarget : public vrender::render::IFrameTarget
	{
	public:
		// Lifetime Control
		RenderPassFrameTarget(
			const vrender::render::Framebuffer& framebuffer,
			const vrender::render::RenderPass& render_pass
		);

		// Interface Methods
		void begin(VkCommandBuffer command_buffer) const override;
		void end(VkCommandBuffer command_buffer) const override;
		const VkExtent2D get_extent() const override;
	private:
		const vrender::render::Framebuffer& framebuffer;
		const vrender::render::RenderPass& render_pass;
	};
}

#endif