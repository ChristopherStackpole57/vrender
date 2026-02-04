#include "SelectPhysicalDevice.h"

#include <iostream>

// Selection Utilities
static bool satisfies_queue_requirements(
	const vrender::render::PhysicalDevice& device,
	const vrender::render::utility::physical_device::QueueRequirements& requirements
)
{
	for (const VkQueueFamilyProperties& family : device.get_queue_families())
	{
		if ((family.queueFlags & requirements.required) == requirements.required)
		{
			return true;
		}
	}
	return false;
}
static bool satisfies_extension_requirements(
	const vrender::render::PhysicalDevice& device,
	const vrender::render::utility::physical_device::ExtensionRequirements& requirements
)
{
	const std::vector<VkExtensionProperties>& available = device.get_extensions();

	for (const std::string& required : requirements.required)
	{
		bool found = false;
		for (const VkExtensionProperties& extension : available)
		{
			if (required == extension.extensionName)
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			return false;
		}
	}

	return true;
}
static bool satisfies_feature_requirements(
	const vrender::render::PhysicalDevice& device,
	const vrender::render::utility::physical_device::FeatureRequirements& requirements
)
{
	const VkBool32* supported = reinterpret_cast<const VkBool32*>(&device.get_features().features);
	const VkBool32* core_required = reinterpret_cast<const VkBool32*>(&requirements.core);

	constexpr size_t count = sizeof(VkPhysicalDeviceFeatures) / sizeof(VkBool32);

	// Check core features
	for (size_t i = 0; i < count; i++)
	{
		if (core_required[i] && !supported[i])
		{
			return false;
		}
	}

	// TODO: Check extended features!

	return true;
}
static bool satisfies_surface_requirements(
	const vrender::render::PhysicalDevice& device,
	std::optional<vrender::render::utility::physical_device::SurfaceRequirements>& requirements
)
{
	if (!requirements || requirements->surface == VK_NULL_HANDLE)
	{
		return false;
	}

	if (!requirements->require_present_queue)
	{
		return true;
	}

	const std::vector<VkQueueFamilyProperties> all_properties =  device.get_queue_families();
	for (size_t i = 0; i < all_properties.size(); i++)
	{
		VkBool32 present_supported = VK_FALSE;
		VkResult check_result = vkGetPhysicalDeviceSurfaceSupportKHR(
			device.get_physical_device(),
			i,
			requirements.value().surface,
			&present_supported
		);

		if (check_result == VK_SUCCESS && present_supported)
		{
			return true;
		}
	}

	return false;
}
static bool satisfies_memory_requirements(
	const vrender::render::PhysicalDevice& device,
	const vrender::render::utility::physical_device::MemoryRequirements requirements
)
{
	if (!requirements.require_device_local)
	{
		return true;
	}

	return device.get_max_device_local_heap_size() >= requirements.min_device_local_bytes;
}

// Score Values
//		GPU Weights
const uint32_t DISCRETE_GPU = 1000;
const uint32_t INTEGRATED_GPU = 300;
const uint32_t VIRTUAL_GPU = 100;
const uint32_t CPU_GPU = 10;

//		Memory Controls
const unsigned long long int STORAGE_CAP = 16;
const uint32_t MEMORY_SCALER = 100;

//		Vendor Weights
const uint32_t VENDOR_NVIDIA = 100;
const uint32_t VENDOR_AMD = 100;
const uint32_t VENDOR_INTEL = 50;

// Physical Device Selector
vrender::render::PhysicalDevice vrender::render::utility::physical_device::select_physical_device(
	std::vector<vrender::render::PhysicalDevice> options,
	vrender::render::utility::physical_device::PhysicalDeviceSelectionParameters selection_parameters
)
{
	const vrender::render::PhysicalDevice* best = nullptr;
	uint32_t best_score = 0;

	for (const vrender::render::PhysicalDevice& device : options)
	{
		// Hard Filtering
		if (!device.is_valid()) continue;
		if (!satisfies_queue_requirements(device, selection_parameters.queues)) continue;
		if (!satisfies_extension_requirements(device, selection_parameters.extensions)) continue;
		if (!satisfies_feature_requirements(device, selection_parameters.features)) continue;
		if (!satisfies_surface_requirements(device, selection_parameters.surface)) continue;
		if (!satisfies_memory_requirements(device, selection_parameters.memory)) continue;

		// Scoring
		uint32_t score = 0;
		VkPhysicalDeviceProperties device_properties;
		vkGetPhysicalDeviceProperties(device.get_physical_device(), &device_properties);

		//		GPU Type
		switch (device_properties.deviceType)
		{
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			score += DISCRETE_GPU;
			break;
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			score += INTEGRATED_GPU;
			break;
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
			score += VIRTUAL_GPU;
			break;
		case VK_PHYSICAL_DEVICE_TYPE_CPU:
			score += CPU_GPU;
			break;
		}

		//		Memory Capability
		score += std::min(device.get_max_device_local_heap_size() / 1000000000, STORAGE_CAP) * MEMORY_SCALER;

		//		TODO: Queue Topology

		//		TODO: Feature Richness

		//		TODO: Performance Properties

		//		TODO: Surface Quality

		//		Vender / Driver Heuristics
		switch (device_properties.vendorID)
		{
		case 0x10DE:											// NVIDIA
			score += VENDOR_NVIDIA;
			break;
		case 0x1002:											// AMD
			score += VENDOR_AMD;
			break;
		case 0x8086:											// INTEL
			score += VENDOR_INTEL;
			break;
		}

		if (score > best_score)
		{
			best_score = score;
			best = &device;
		}
	}

	// Selection Result
	return *best;
}