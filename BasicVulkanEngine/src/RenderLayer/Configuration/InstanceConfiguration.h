#ifndef RENDER_INSTANCE_CONFIGURATION_H
#define RENDER_INSTANCE_CONFIGURATION_H

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include "PlatformLayer/WindowBackends/WindowSurfaceProvider.h"

namespace vrender::render::config
{
	struct InstanceConfig
	{
		// Configuration for Create Info
		std::vector<std::string> extensions;
		std::vector<std::string> layers;
		std::vector<std::string> validation_layers{ "VK_LAYER_KHRONOS_validation" };
		bool enable_validation = true;

		// Configuration for Application Info
		std::string application_name = "VulkanRendering";
		uint32_t application_version = VK_MAKE_VERSION(1, 0, 0);
		std::string engine_name = "BasicVulkanEngine";
		uint32_t engine_version = VK_MAKE_VERSION(1, 0, 0);
		uint32_t api_version = VK_API_VERSION_1_2;
	};
}

#endif