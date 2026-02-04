#include "PhysicalDeviceEnumeration.h"

std::vector<vrender::render::PhysicalDevice> vrender::render::utility::physical_device::enumerate_physical_devices(
	const vrender::render::Instance& instance
)
{
	std::vector<VkPhysicalDevice> raw_devices = instance.query_physical_devices();
	std::vector<vrender::render::PhysicalDevice> physical_devices;
	physical_devices.reserve(raw_devices.size());

	for (VkPhysicalDevice raw_device : raw_devices)
	{
		physical_devices.emplace_back(raw_device);
	}

	return physical_devices;
}