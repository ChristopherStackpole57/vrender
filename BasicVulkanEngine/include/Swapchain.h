#ifndef RENDER_SWAPCHAIN_H
#define RENDER_SWAPCHAIN_H

#include <algorithm>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include <LogicalDevice.h>
#include <Semaphore.h>

#include <RenderLayer/Utility/ConfigureSwapchainCreateInfo.h>
#include <RenderLayer/Utility/QuerySwapchainSupport.h>

namespace vrender::render
{
	struct AcquireSwapchainImageResult
	{
		VkResult result;
		uint32_t image_index;
	};

	class Swapchain
	{
	public:
		// Lifetime Control
		Swapchain(
			const vrender::render::LogicalDevice& logical_device,
			const VkSurfaceKHR surface,
			const vrender::render::utility::swapchain::SwapchainConfiguration configuration
		);
		~Swapchain();

		// API Accessibility
		//		Configuration Access
		const VkFormat get_image_format() const;
		const VkColorSpaceKHR get_color_space() const;
		const VkExtent2D get_extent() const;
		const uint32_t get_image_count() const;

		//		Image and View Access
		const std::vector<VkImage> get_images() const;
		const std::vector<VkImageView> get_image_views() const;
		const VkImage get_image(uint32_t index) const;
		const VkImageView get_image_view(uint32_t index) const;

		//		Image Acquisition
		const vrender::render::AcquireSwapchainImageResult acquire_image(const vrender::render::Semaphore& semaphore, uint64_t timeout) const;

		//		Presentation
		// present image, requires synchronization handles

		//		Recreation
		bool needs_recreation(VkResult last_result) const;

		const VkSwapchainKHR get_swapchain() const;
	private:
		VkSwapchainKHR swapchain = VK_NULL_HANDLE;
		const vrender::render::LogicalDevice* device_ptr = nullptr;

		std::vector<VkImage> images;
		std::vector<VkImageView> image_views;
	
		// Cached Information
		VkFormat image_format;
		VkColorSpaceKHR color_space;
		VkExtent2D image_extent;
		uint32_t image_count;
	};
}

#endif