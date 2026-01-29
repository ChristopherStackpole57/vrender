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
}
vrender::render::Swapchain::~Swapchain()
{
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