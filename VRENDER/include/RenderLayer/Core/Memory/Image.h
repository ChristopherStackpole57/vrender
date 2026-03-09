#ifndef RENDER_IMAGE_H
#define RENDER_IMAGE_H

#include <vulkan/vulkan.h>

#include <RenderLayer/Core/Memory/Allocator.h>
#include <RenderLayer/Core/Memory/ImageDesc.h>

namespace vrender::render::memory
{
	class Image
	{
	public:
		// Lifetime Control
		Image(
			vrender::render::memory::Allocator& allocator,
			vrender::render::memory::ImageDesc& desc
		);
		~Image();

		Image(const Image&) = delete;
		Image& operator=(const Image&) = delete;

		Image(Image&& other) noexcept;
		Image& operator=(Image&& other) noexcept;

		// API Accessbility
		VkImage get_image() const;
		//VkDeviceSize get_size() const;

		void write();
	private:
		VkImage image;
		vrender::render::memory::ImageDesc desc;
		vrender::render::memory::AllocationHandle handle;

		vrender::render::memory::Allocator* allocator_ptr;
	};
}

#endif