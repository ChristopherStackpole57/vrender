#include "PhysicalDevice.h"

// Lifecycle Control
vrender::render::PhysicalDevice::PhysicalDevice(VkPhysicalDevice physical_device) : device(physical_device)
{
	assert(this->device != VK_NULL_HANDLE);

	// FEATURES AND PROPERTIES
	features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
	features.pNext = nullptr;

	properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
	features.pNext = nullptr;

	vkGetPhysicalDeviceFeatures2(this->device, &this->features);
	vkGetPhysicalDeviceProperties2(this->device, &this->properties);

	// QUEUE FAMILY PROPERTIES
	uint32_t queue_family_properties_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties2(this->device, &queue_family_properties_count, nullptr);
	if (queue_family_properties_count == 0)
	{
		std::cerr << "[RENDER] Vulkan Encountered and Disabled a Physical Device with No Queue Families" << std::endl;
		this->valid = false;
		return;
	}

	this->queue_family_properties.resize(queue_family_properties_count);
	this->queue_family_properties2.resize(queue_family_properties_count);
	vkGetPhysicalDeviceQueueFamilyProperties(this->device, &queue_family_properties_count, this->queue_family_properties.data());
	vkGetPhysicalDeviceQueueFamilyProperties2(this->device, &queue_family_properties_count, this->queue_family_properties2.data());
	
	// DEVICE EXTENSIONS
	uint32_t extension_count = 0;
	VkResult enumeration_result = vkEnumerateDeviceExtensionProperties(this->device, nullptr, &extension_count, nullptr);
	if (enumeration_result != VK_SUCCESS)
	{
		std::cerr << "[RENDER] Vulkan Encounted and Disabled a Physical Device Unable to Enumerate Extension Properties" << std::endl;
		this->valid = false;
		return;
	}

	if (extension_count == 0)
	{
		std::cerr << "[RENDER] Vulkan Encountered and Disabled a Physical Device With No Extensions" << std::endl;
		this->valid = false;
		return;
	}
	this->extension_properties.resize(extension_count);

	enumeration_result = vkEnumerateDeviceExtensionProperties(this->device, nullptr, &extension_count, this->extension_properties.data());
	if (enumeration_result != VK_SUCCESS)
	{
		std::cerr << "[RENDER] Vulkan Encountered and Disabled a Physical Device Unable to Enumerate Extension Properties" << std::endl;
		this->valid = false;
		return;
	}

	// MEMORY PROPERTIES
	vkGetPhysicalDeviceMemoryProperties2(this->device, &this->memory_properties);
}

// API Accessibility
const VkPhysicalDevice vrender::render::PhysicalDevice::get_physical_device() const
{
	return device;
}
const std::vector<VkQueueFamilyProperties>& vrender::render::PhysicalDevice::get_queue_families() const
{
	return this->queue_family_properties;
}
const std::vector<VkExtensionProperties>& vrender::render::PhysicalDevice::get_extensions() const
{
	return this->extension_properties;
}
const std::vector<const char*> vrender::render::PhysicalDevice::get_raw_extension_names() const
{
	std::vector<const char*> names;
	for (const VkExtensionProperties& extension : this->extension_properties)
	{
		const char* name = extension.extensionName;
		names.push_back(name);
	}

	return names;
}
const std::vector<std::string> vrender::render::PhysicalDevice::get_extension_names() const
{
	std::vector<std::string> names;
	for (const VkExtensionProperties& extension : this->extension_properties)
	{
		names.push_back(std::string(extension.extensionName));
	}

	return names;
}
const VkPhysicalDeviceFeatures2& vrender::render::PhysicalDevice::get_features() const
{
	return this->features;
}
uint64_t vrender::render::PhysicalDevice::get_max_device_local_heap_size() const
{
	uint64_t max_size = 0;
	const VkPhysicalDeviceMemoryProperties& mem = this->memory_properties.memoryProperties;
	
	for (int i = 0; i < mem.memoryHeapCount; i++)
	{
		const VkMemoryHeap& heap = mem.memoryHeaps[i];
		
		if (heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
		{
			max_size = std::max(max_size, heap.size);
		}
	}

	return max_size;
}

bool vrender::render::PhysicalDevice::supports_feature(const VkPhysicalDeviceFeatures& required) const
{
	const VkPhysicalDeviceFeatures& available = this->features.features;

	const VkBool32* req = reinterpret_cast<const VkBool32*>(&required);
	const VkBool32* avail = reinterpret_cast<const VkBool32*>(&available);

	constexpr size_t count = sizeof(VkPhysicalDeviceFeatures) / sizeof(VkBool32);

	for (size_t i = 0; i < count; i++)
	{
		if (req[i] == VK_TRUE && avail[i] != VK_TRUE)
		{
			return false;
		}
	}

	return true;
}

bool vrender::render::PhysicalDevice::is_valid() const
{
	return this->valid;
}

const std::string vrender::render::PhysicalDevice::get_name() const
{
	return this->properties.properties.deviceName;
}