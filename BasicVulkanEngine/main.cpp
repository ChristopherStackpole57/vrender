#include <iostream>

#include "Configuration/VulkanConfig.h"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "Source/RenderLayer/Instance.h"
#include "Source/RenderLayer/InstanceConfiguration.h"

int main()
{
	vrender::render::InstanceConfig config;
	config.extensions = std::vector<std::string>{
		"VK_KHR_surface",
		"VK_KHR_win32_surface"
	};
	vrender::render::Instance instance(config);

	return 0;
}