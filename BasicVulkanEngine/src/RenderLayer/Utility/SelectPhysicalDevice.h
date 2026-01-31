#ifndef RENDER_UTILITY_SELECT_PHYSICAL_DEVICE_H
#define RENDER_UTILITY_SELECT_PHYSICAL_DEVICE_H

#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

#include <PhysicalDevice.h>

namespace vrender::render
{
	namespace utility::physical_device
	{
		struct QueueRequirements
		{
			VkQueueFlags required =
				VK_QUEUE_GRAPHICS_BIT	|
				VK_QUEUE_COMPUTE_BIT	|
				VK_QUEUE_TRANSFER_BIT;
		};
		struct ExtensionRequirements
		{
			std::vector<std::string> required;
		};
		struct FeatureRequirements
		{
			VkPhysicalDeviceFeatures core{};
			std::vector<std::unique_ptr<VkBaseOutStructure>> extended;
		};
		struct SurfaceRequirements
		{
			VkSurfaceKHR surface = VK_NULL_HANDLE;
			bool require_present_queue = true;
		};
		struct MemoryRequirements
		{
			bool require_device_local = true;
			uint64_t min_device_local_bytes = 0;
		};

		struct PhysicalDeviceSelectionParameters
		{
			QueueRequirements queues;
			ExtensionRequirements extensions;
			FeatureRequirements features;
			std::optional<SurfaceRequirements> surface;
			MemoryRequirements memory;
		};

		vrender::render::PhysicalDevice select_physical_device(
			std::vector<PhysicalDevice> options,
			vrender::render::utility::physical_device::PhysicalDeviceSelectionParameters selection_parameters
		);
	}
}

#endif