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
	// TODO: This algorithm could use some improvement overall
	vrender::render::utility::queue::QueueSelection selection_result{};

	const std::vector<VkQueueFamilyProperties>& device_queue_family_properties = physical_device.get_queue_families();

	// Look for a unified queue
	uint32_t index = 0;
	for (const VkQueueFamilyProperties& queue_family_properties : device_queue_family_properties)
	{
		if (
			queue_family_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT &&
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

	// Look for minimal splits, Graphics + Present, Compute Seperate
	index = 0;
	for (const VkQueueFamilyProperties& queue_family_properties : device_queue_family_properties)
	{
		if (
			queue_family_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT &&
			queue_family_properties.queueFlags & VK_QUEUE_TRANSFER_BIT &&
			queue_family_supports_present(
				physical_device,
				surface,
				index
			) &&
			queue_family_properties.queueCount > 0
			)
		{
			selection_result.graphics_family = index;
			selection_result.present_family = index;
			break;
		}
		index++;

		if (
			queue_family_properties.queueFlags & VK_QUEUE_COMPUTE_BIT &&
			queue_family_properties.queueFlags & VK_QUEUE_TRANSFER_BIT &&
			queue_family_properties.queueCount > 0
			)
		{
			selection_result.compute_family = index;
		}
	}

	// Reset Selection Result iff there was no queue family found for Graphics + Present
	if (selection_result.graphics_family == INT_MAX) selection_result = vrender::render::utility::queue::QueueSelection{};
	else return selection_result;

	// Look for seperate Graphics, Present, and Compute
	index = 0;
	for (const VkQueueFamilyProperties& queue_family_properties : device_queue_family_properties)
	{
		if (!(queue_family_properties.queueFlags & VK_QUEUE_TRANSFER_BIT) || !(queue_family_properties.queueCount > 0)) continue;
		if (queue_family_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT) selection_result.graphics_family = index;
		if (queue_family_properties.queueFlags & VK_QUEUE_COMPUTE_BIT) selection_result.compute_family = index;
		if (queue_family_supports_present(
			physical_device,
			surface,
			index
		)) selection_result.present_family = index;

		index++;
	}

	if (selection_result.graphics_family == INT_MAX || selection_result.present_family == INT_MAX)
	{
		// If either graphics or present isn't availible, return only the compute
		selection_result.graphics_family = INT_MAX;
		selection_result.present_family = INT_MAX;
	}
	return selection_result;
}