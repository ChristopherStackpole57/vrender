#ifndef RENDER_INTERFACE_FRAME_TARGET_H
#define RENDER_INTERFACE_FRAME_TARGET_H

#include <vulkan/vulkan.h>

namespace vrender::render
{
	class IFrameTarget
	{
	public:
		virtual void begin(VkCommandBuffer command_buffer) const = 0;
		virtual void end(VkCommandBuffer command_buffer) const = 0;
		virtual const VkExtent2D get_extent() const = 0;
	private:

	};
}

#endif