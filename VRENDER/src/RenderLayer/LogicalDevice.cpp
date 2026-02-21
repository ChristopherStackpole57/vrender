#include <Core/LogicalDevice.h>

// Lifetime Control
vrender::render::LogicalDevice::LogicalDevice(
	const vrender::render::PhysicalDevice& physical_device,
	const vrender::render::utility::queue::QueueSelection& queue_selection_result,
	const std::vector<std::string>& required_extensions
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

	this->enabled_extensions = required_extensions;
	std::vector<const char*> extension_names;
	extension_names.reserve(required_extensions.size());
	for (const std::string& extension_name : required_extensions)
	{
		extension_names.push_back(extension_name.c_str());
	}

	VkPhysicalDeviceFeatures2 device_features = physical_device.get_features();

	// Enable Dynamic Rendering and Synch2
	VkPhysicalDeviceVulkan13Features features13{};
	features13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
	features13.dynamicRendering = VK_TRUE;
	features13.synchronization2 = VK_TRUE;
	device_features.pNext = &features13;

	// Enable Descriptor Indexing
	VkPhysicalDeviceDescriptorIndexingFeatures indexing{};
	indexing.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
	indexing.runtimeDescriptorArray = VK_TRUE;
	indexing.descriptorBindingPartiallyBound = VK_TRUE;
	indexing.descriptorBindingVariableDescriptorCount = VK_TRUE;
	indexing.descriptorBindingUpdateUnusedWhilePending = VK_TRUE;
	indexing.descriptorBindingStorageBufferUpdateAfterBind = VK_TRUE;
	indexing.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
	features13.pNext = &indexing;

	VkDeviceCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	create_info.pNext = &device_features;
	create_info.flags = 0;
	create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
	create_info.pQueueCreateInfos = queue_create_infos.data();
	create_info.enabledExtensionCount = static_cast<uint32_t>(extension_names.size());
	create_info.ppEnabledExtensionNames = extension_names.data();
	create_info.pEnabledFeatures = nullptr;

	VkResult creation_result = vkCreateDevice(
		physical_device.get_physical_device(), 
		&create_info,
		nullptr,
		&this->device
	);
	std::cout << creation_result << std::endl;
	if (creation_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Device Creation Failed");
	}

	// Store Queues
	for (uint32_t family_index : unique_families)
	{
		VkQueueFlags capabilities = device_queue_family_properties[family_index].queueFlags;
		VkQueue queue;
		vkGetDeviceQueue(
			this->device,
			family_index,
			0,
			&queue
		);
		vrender::render::utility::queue::QueueHandle queue_handle{
			.queue = queue,
			.family_index = family_index,
			.index = 0,
			.capabilities = capabilities
		};
		this->queues.emplace_back(queue_handle);
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
VkDevice vrender::render::LogicalDevice::get_logical_device() const
{
	return this->device;
}
VkQueue vrender::render::LogicalDevice::get_graphics_queue() const
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
VkQueue vrender::render::LogicalDevice::get_compute_queue() const
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

// TODO: These can be made faster by caching family indices
uint32_t vrender::render::LogicalDevice::get_graphics_queue_family_index() const
{
	for (vrender::render::utility::queue::QueueHandle queue : this->queues)
	{
		if (queue.supports_graphics())
		{
			return queue.family_index;
		}
	}
	return UINT32_MAX;
}
uint32_t vrender::render::LogicalDevice::get_compute_queue_family_index() const
{
	for (vrender::render::utility::queue::QueueHandle queue : this->queues)
	{
		if (queue.supports_compute())
		{
			return queue.family_index;
		}
	}
}

const std::vector<std::string>& vrender::render::LogicalDevice::get_enabled_extensions() const
{
	return this->enabled_extensions;
}