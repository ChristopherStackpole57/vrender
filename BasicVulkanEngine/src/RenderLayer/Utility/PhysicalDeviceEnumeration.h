#ifndef RENDER_UTILITY_PHYSICAL_DEVICE_ENUMERATION_H
#define RENDER_UTILITY_PHYSICAL_DEVICE_ENUMERATION_H

#include <vulkan/vulkan.h>

#include <Instance.h>
#include <PhysicalDevice.h>

namespace vrender::render
{
	namespace utility::physical_device
	{
		std::vector<vrender::render::PhysicalDevice> enumerate_physical_devices(const Instance& instance);
	}
}

#endif