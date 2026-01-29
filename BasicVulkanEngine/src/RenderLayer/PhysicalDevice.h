#ifndef RENDER_PHYSICAL_DEVICE_H
#define RENDER_PHYSICAL_DEVICE_H

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

namespace vrender::render
{
	class PhysicalDevice
	{
	public:
		// Lifecycle Control
		PhysicalDevice(VkPhysicalDevice device);
		~PhysicalDevice() = default;

		// API Accessibility
		const VkPhysicalDevice get_physical_device() const;
		const std::vector<VkQueueFamilyProperties>& get_queue_families() const;
		const std::vector<VkExtensionProperties>& get_extensions() const;
		const std::vector<const char*> get_raw_extension_names() const;
		const std::vector<std::string> get_extension_names() const;
		const VkPhysicalDeviceFeatures2& get_features() const;
		VkDeviceSize get_max_device_local_heap_size() const;

		bool supports_feature(const VkPhysicalDeviceFeatures& required) const;

		bool is_valid() const;

		const std::string get_name() const;
	private:
		bool valid = true;

		VkPhysicalDevice device;
		VkPhysicalDeviceFeatures2 features{};
		VkPhysicalDeviceProperties2 properties{};
		std::vector<VkQueueFamilyProperties> queue_family_properties;
		std::vector<VkQueueFamilyProperties2> queue_family_properties2;
		std::vector<VkExtensionProperties> extension_properties;
		VkPhysicalDeviceMemoryProperties2 memory_properties{};
	};
}

#endif