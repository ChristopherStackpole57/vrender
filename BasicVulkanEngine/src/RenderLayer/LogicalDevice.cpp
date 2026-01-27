#include "LogicalDevice.h"

// Lifetime Control
vrender::render::LogicalDevice::LogicalDevice(
	const vrender::render::PhysicalDevice& physical_device,
	const vrender::render::utility::queue::QueueSelection& queue_selection_result
)
{
	// Verify that a physical device has been given
	assert(physical_device.get_physical_device() != VK_NULL_HANDLE);

	// Generate Queue Family Create Info
	const std::vector<VkQueueFamilyProperties> device_queue_family_properties = physical_device.get_queue_families();
	
	std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
	queue_create_infos.reserve(device_queue_family_properties.size());
	
	std::vector<std::vector<float>> queue_priorities;

	std::set<uint32_t> unique_families = {
		queue_selection_result.graphics_family,
		queue_selection_result.present_family,
		queue_selection_result.compute_family
	};

	for (const uint32_t family_index : unique_families)
	{
		const VkQueueFamilyProperties& family_properties = device_queue_family_properties[family_index];
		queue_priorities.emplace_back(family_properties.queueCount, 1.0f);

		VkDeviceQueueCreateInfo queue_create_info{};
		queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_create_info.pNext = nullptr;
		queue_create_info.flags = 0;
		queue_create_info.queueFamilyIndex = family_index;
		queue_create_info.queueCount = 1;											// TODO: Profiling for the right number of queues to hold
		queue_create_info.pQueuePriorities = queue_priorities.back().data();

		queue_create_infos.emplace_back(queue_create_info);
	}

	// How do we know that these extension names are valid?
	this->enabled_extensions = physical_device.get_extension_names();
	const std::vector<const char*> extension_names = physical_device.get_raw_extension_names();
	const VkPhysicalDeviceFeatures2& device_features = physical_device.get_features();

	VkDeviceCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	create_info.pNext = &device_features;
	create_info.flags = 0;
	create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
	create_info.pQueueCreateInfos = queue_create_infos.data();
	create_info.enabledExtensionCount = static_cast<uint32_t>(extension_names.size());
	create_info.ppEnabledExtensionNames = extension_names.data();
	create_info.pEnabledFeatures = nullptr;

	VkResult creation_reult = vkCreateDevice(
		physical_device.get_physical_device(), 
		&create_info,
		nullptr,
		&this->device
	);

	if (creation_reult != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Device Creation Failed");
	}

	// Store Queues
	for (uint32_t family_index : unique_families)
	{
		VkQueueFlags capabilities = device_queue_family_properties[family_index].queueFlags;
		for (uint32_t i = 0; i < device_queue_family_properties[family_index].queueCount; i++)
		{
			VkQueue queue;
			vkGetDeviceQueue(
				this->device,
				family_index,
				i,
				&queue
			);
			vrender::render::utility::queue::QueueHandle queue_handle{
				.queue = queue,
				.family_index = family_index,
				.index = i,
				.capabilities = capabilities
			};
			this->queues.emplace_back(queue_handle);
		}
	}
}
vrender::render::LogicalDevice::~LogicalDevice()
{
	if (this->device == VK_NULL_HANDLE)
	{
		return;
	}

	vkDeviceWaitIdle(this->device);
	vkDestroyDevice(
		this->device,
		nullptr
	);
	this->device = VK_NULL_HANDLE;
}

// API Accessibility
const VkDevice vrender::render::LogicalDevice::get_logical_device() const
{
	return this->device;
}
const VkQueue vrender::render::LogicalDevice::get_graphics_queue() const
{
	for (vrender::render::utility::queue::QueueHandle queue : this->queues)
	{
		if (queue.supports_graphics())
		{
			return queue.queue;
		}
	}
	return VK_NULL_HANDLE;
}
const VkQueue vrender::render::LogicalDevice::get_compute_queue() const
{
	for (vrender::render::utility::queue::QueueHandle queue : this->queues)
	{
		if (queue.supports_compute())
		{
			return queue.queue;
		}
	}
	return VK_NULL_HANDLE;
}
const std::vector<std::string>& vrender::render::LogicalDevice::get_enabled_extensions() const
{
	return this->enabled_extensions;
}