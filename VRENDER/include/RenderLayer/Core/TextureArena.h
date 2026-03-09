#ifndef RENDER_TEXTURE_ARENA_H
#define RENDER_TEXTURE_ARENA_H

#include <string>

#include <vulkan/vulkan.h>
#include <stb_image/stb_image.h>

#include <RenderLayer/Core/Texture.h>

#include <RenderLayer/Core/Memory/Allocator.h>
#include <RenderLayer/Core/Memory/Buffer.h>
#include <RenderLayer/Core/Memory/CPUAccess.h>
#include <RenderLayer/Core/Memory/Image.h>
#include <RenderLayer/Core/Memory/ImageDesc.h>
#include <RenderLayer/Core/Memory/Suballocator.h>

#include <Utility/Generator.h>

namespace vrender::render
{
	class TextureArena
	{
	public:
		// Lifetime Control
		TextureArena(
			vrender::render::LogicalDevice& logical_device,
			vrender::render::memory::Allocator& allocator
		);
		~TextureArena();

		TextureArena(const TextureArena&) = delete;
		TextureArena& operator=(const TextureArena&) = delete;

		// API Accessibility
		const vrender::render::Texture create_texture(std::string path);
	private:
		uint32_t arena_length = 1024 * 1024 * 4;

		vrender::render::memory::Buffer texture_buffer;
		vrender::render::memory::Suballocator texture_suballocator;

		vrender::utility::Generator<vrender::render::TextureEntry> generator;

		vrender::render::memory::Allocator* allocator_ptr;
		vrender::render::LogicalDevice* logical_device_ptr;
	};
}

#endif