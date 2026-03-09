#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include <vulkan/vulkan.h>

#include <Utility/Generator.h>

namespace vrender::render
{
	struct Texture
	{
		VkImage image;
		VkImageView view;
		VkSampler sampler;
	};

	struct TextureEntry
	{
		vrender::render::Texture texture;
	};
	typedef vrender::utility::Handle TextureHandle;
}

#endif