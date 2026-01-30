#include "Swapchain.h"

// Lifetime Control
vrender::render::Swapchain::Swapchain(
	const vrender::render::LogicalDevice& logical_device,
	const VkSurfaceKHR surface,
	const vrender::render::utility::swapchain::SwapchainConfiguration configuration
) 
	: device_ptr(&logical_device)
{
	VkSwapchainCreateInfoKHR create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	create_info.pNext = nullptr;
	create_info.flags = 0;
	create_info.surface = surface;
	create_info.minImageCount = configuration.min_image_count;
	create_info.imageFormat = configuration.image_format;
	create_info.imageColorSpace = configuration.image_color_space;
	create_info.imageExtent = configuration.image_extent;
	create_info.imageArrayLayers = configuration.image_array_layers;
	create_info.imageUsage = configuration.image_usage;
	create_info.imageSharingMode = configuration.image_sharing_mode;
	create_info.queueFamilyIndexCount = configuration.queue_family_index_count;
	create_info.pQueueFamilyIndices = (configuration.queue_family_indices.empty())
		? nullptr
		: configuration.queue_family_indices.data();
	create_info.preTransform = configuration.pre_transform;
	create_info.compositeAlpha = configuration.composite_alpha;
	create_info.presentMode = configuration.present_mode;
	create_info.clipped = configuration.clipped;
	create_info.oldSwapchain = configuration.old_swapchain;

	VkResult creation_result = vkCreateSwapchainKHR(
		logical_device.get_logical_device(),
		&create_info,
		nullptr,
		&this->swapchain
	);
	if (creation_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Failed to Create Swapchain");
	}

	// Cache Information
	this->image_format = configuration.image_format;
	this->color_space = configuration.image_color_space;
	this->image_extent = configuration.image_extent;
	this->image_count = configuration.min_image_count;

	// Query Images from Swapchain
	uint32_t count = 0;
	VkResult query_result = vkGetSwapchainImagesKHR(
		logical_device.get_logical_device(),
		this->swapchain,
		&count,
		nullptr
	);
	if (query_result != VK_SUCCESS || count == 0)
	{
		throw std::runtime_error("ERROR: Vulkan Swapchain Unable to Query Image Count");
	}

	this->images.resize(count);
	query_result = vkGetSwapchainImagesKHR(
		logical_device.get_logical_device(),
		this->swapchain,
		&count,
		this->images.data()
	);
	if (query_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Swapchain Unable to Query Images");
	}
	
	// Create Image Views for Queried Images
	this->image_views.reserve(count);
	for (const VkImage image : this->images)
	{
		VkImageViewCreateInfo create_info{};
		create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		create_info.pNext = nullptr;
		create_info.flags = 0;
		create_info.image = image;
		create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		create_info.format = this->image_format;

		create_info.components.r = VK_COMPONENT_SWIZZLE_R;
		create_info.components.g = VK_COMPONENT_SWIZZLE_G;
		create_info.components.b = VK_COMPONENT_SWIZZLE_B;
		create_info.components.a = VK_COMPONENT_SWIZZLE_A;

		create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		create_info.subresourceRange.baseMipLevel = 0;
		create_info.subresourceRange.levelCount = 1;
		create_info.subresourceRange.baseArrayLayer = 0;
		create_info.subresourceRange.layerCount = 1;

		VkImageView image_view = VK_NULL_HANDLE;
		VkResult creation_result = vkCreateImageView(
			logical_device.get_logical_device(),
			&create_info,
			nullptr,
			&image_view
		);
		if (creation_result != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Vulkan Unable to Create Swapchain Image View");
		}

		this->image_views.emplace_back(image_view);
	}
}
vrender::render::Swapchain::~Swapchain()
{
	if (!(this->image_views.empty()))
	{
		for (const VkImageView image_view : this->image_views)
		{
			vkDestroyImageView(
				this->device_ptr->get_logical_device(),
				image_view,
				nullptr
			);
		}
	}

	if (this->swapchain == VK_NULL_HANDLE)
	{
		return;
	}

	vkDestroySwapchainKHR(
		this->device_ptr->get_logical_device(),
		this->swapchain,
		nullptr
	);
}

// API Accessibility
//		Configuration Access
const VkFormat vrender::render::Swapchain::get_image_format() const
{
	return this->image_format;
}
const VkColorSpaceKHR vrender::render::Swapchain::get_color_space() const
{
	return this->color_space;
}
const VkExtent2D vrender::render::Swapchain::get_extent() const
{
	return this->image_extent;
}
const uint32_t vrender::render::Swapchain::get_image_count() const
{
	return this->image_count;
}

//		Image and View Access
const std::vector<VkImage> vrender::render::Swapchain::get_images() const
{
	return this->images;
}
const std::vector<VkImageView> vrender::render::Swapchain::get_image_views() const
{
	return this->image_views;
}
const VkImage vrender::render::Swapchain::get_image(uint32_t index) const
{
	if (index >= this->images.size())
	{
		return VK_NULL_HANDLE;
	}
	return this->images[index];
}
const VkImageView vrender::render::Swapchain::get_image_view(uint32_t index) const
{
	if (index >= this->image_views.size())
	{
		return VK_NULL_HANDLE;
	}
	return this->image_views[index];
}

//		Image Acquisition
const vrender::render::AcquireSwapchainImageResult vrender::render::Swapchain::acquire_image(const vrender::render::Semaphore& semaphore, uint64_t timeout) const
{
	// The Acquisition Result is only valid if image_index != UINT32_MAX
	uint32_t index = UINT32_MAX;

	VkResult acquisition_result = vkAcquireNextImageKHR(
		this->device_ptr->get_logical_device(),
		this->swapchain,
		timeout,
		semaphore.get_semaphore(),
		VK_NULL_HANDLE,
		&index
	);

	return {
		.result = acquisition_result, 
		.image_index = index 
	};
}

//		Presentation
// TODO: Implement, requires synchronization

//		Recreation
bool vrender::render::Swapchain::needs_recreation(VkResult last_result) const
{
	// TODO: Implement
	return true;
}

const VkSwapchainKHR vrender::render::Swapchain::get_swapchain() const
{
	return this->swapchain;
}