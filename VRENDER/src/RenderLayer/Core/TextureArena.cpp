#include <RenderLayer/Core/TextureArena.h>

// Lifetime Control
vrender::render::TextureArena::TextureArena(
	vrender::render::LogicalDevice& logical_device,
	vrender::render::memory::Allocator& allocator
)
	: texture_buffer(
		allocator,
		this->arena_length,
		vrender::render::memory::BufferUsageClass::STORAGE,
		vrender::render::memory::CPUAccess::WRITE_OFTEN,
		vrender::render::memory::BufferLifetime::PERSISTENT
	)
	, texture_suballocator(
		vrender::render::memory::SuballocatorStrategy::FREE_LIST,
		0,
		this->arena_length
	)
	, allocator_ptr(&allocator)
	, logical_device_ptr(&logical_device)
{

}
vrender::render::TextureArena::~TextureArena()
{

}

// API Accessibility
const vrender::render::Texture vrender::render::TextureArena::create_texture(std::string path)
{
	// Create a New Texture
	vrender::render::Texture texture;

	//		Load Image From Disk
	int width, height, channels;
	stbi_uc* pixels = stbi_load(
		path.c_str(),
		&width,
		&height,
		&channels,
		STBI_rgb_alpha
	);
	uint32_t size = width * height * 4;

	//		Create Image
	vrender::render::memory::ImageDesc desc{};
	desc.width = width;
	desc.height = height;
	desc.format = VK_FORMAT_R8G8B8A8_SRGB;
	desc.usage =
		VK_IMAGE_USAGE_TRANSFER_DST_BIT |
		VK_IMAGE_USAGE_SAMPLED_BIT;
	desc.cpu_access = vrender::render::memory::CPUAccess::NONE;

	vrender::render::memory::ImageAllocationResult alloc_result = this->allocator_ptr->allocate_image(desc);

	//		Create staging buffer
	vrender::render::memory::Buffer staging_buffer(
		*(this->allocator_ptr),
		size,
		vrender::render::memory::BufferUsageClass::STAGING,
		vrender::render::memory::CPUAccess::WRITE_ONCE,
		vrender::render::memory::BufferLifetime::TRANSIENT
	);

	staging_buffer.write(
		pixels,
		size,
		0
	);

	stbi_image_free(pixels);



	// Create Upload / Staging Path
	// Transfer data from Staging into Image
	// Resource Barriers to enforce Layout








	//		Create Vulkan Image View
	VkImageViewCreateInfo view_info{};

	vkCreateImageView(
		this->logical_device_ptr->get_logical_device(),
		&view_info,
		nullptr,
		&texture.view
	);

	VkSamplerCreateInfo sampler_info{};

	vkCreateSampler(
		this->logical_device_ptr->get_logical_device(),
		&sampler_info,
		nullptr,
		&texture.sampler
	);

	return {};
}