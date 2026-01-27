#include "QueueSelection.h"

bool queue_family_supports_present(
	const vrender::render::PhysicalDevice& physical_device,
	const VkSurfaceKHR surface,
	const uint32_t queue_family_index
)
{
	// Verify queue family index is viable
	assert(queue_family_index < physical_device.get_queue_families().size());

	VkBool32 supported = false;
	VkResult device_supports_surface = vkGetPhysicalDeviceSurfaceSupportKHR(
		physical_device.get_physical_device(),
		queue_family_index,
		surface,
		&supported
	);

	if (device_supports_surface != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Failed to Query Phyical Device Surface Support");
	}
	return supported;
}

vrender::render::utility::queue::QueueSelection vrender::render::utility::queue::select_queue_families(
	const vrender::render::PhysicalDevice& physical_device,
	const VkSurfaceKHR& surface
)
{
	vrender::render::utility::queue::QueueSelection selection_result{};

	const std::vector<VkQueueFamilyProperties>& device_queue_family_properties = physical_device.get_queue_families();

	// Look for a unified queue
	uint32_t index = 0;
	for (const VkQueueFamilyProperties& queue_family_properties : device_queue_family_properties)
	{
		if (queue_family_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT &&
			queue_family_properties.queueFlags & VK_QUEUE_COMPUTE_BIT &&
			queue_family_properties.queueFlags & VK_QUEUE_TRANSFER_BIT &&
			queue_family_supports_present(
				physical_device,
				surface,
				index
			) &&
			queue_family_properties.queueCount > 0
		)
		{
			return vrender::render::utility::queue::QueueSelection{ index, index, index };
		}
		index++;
	}

	// Look for minimal splits
	//		Graphics + Present, Seperate Compute
	//		Seperate Graphics, Present, and Compute
	//		Compute Only
}