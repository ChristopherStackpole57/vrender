#include "Instance.h"

// Lifecycle Management
vrender::render::Instance::Instance(const vrender::render::InstanceConfig& config)
{
	// Before creating the Vulkan instance create info, validate that the requested extensions exist.
	check_extension_list_support(config.extensions);

	std::vector<const char*> extension_cstrs;
	extension_cstrs.reserve(config.extensions.size());
	for (const std::string& extension : config.extensions)
	{
		extension_cstrs.push_back(extension.c_str());
	}

	VkApplicationInfo application_info{};
	application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	application_info.pNext = nullptr;
	application_info.pApplicationName = config.application_name.c_str();
	application_info.applicationVersion = config.application_version;
	application_info.pEngineName = config.engine_name.c_str();
	application_info.engineVersion = config.engine_version;
	application_info.apiVersion = config.api_version;

	VkInstanceCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pNext = nullptr;
	create_info.flags = 0;
	create_info.pApplicationInfo = &application_info;
	create_info.enabledExtensionCount = static_cast<uint32_t>(extension_cstrs.size());
	create_info.ppEnabledExtensionNames = extension_cstrs.data();

	// Create the VkInstance using the information already described.
	VkResult creation_result = vkCreateInstance(
		&create_info,
		nullptr,
		&(this->instance)
	);

	if (creation_result != VK_SUCCESS)
	{
		// In this case, creating the VkInstance has failed and the engine must clearly reflect that. For now that is done with a runtime_error.
		throw std::runtime_error("ERROR: Vulkan Instance Creation Failed");
	}
	else
	{
		std::cout << "[Render] Vulkan Instance Created" << std::endl;
		std::cout << "[Render] Extensions Enabled: " << std::endl;
		for (const std::string& extension : config.extensions)
		{
			std::cout << " - " << extension << std::endl;
		}
	}
}
vrender::render::Instance::~Instance()
{
	if (this->instance == VK_NULL_HANDLE)
	{
		// The stored VkInstance is invalid. In this case, vkDestroyInstance will fail.
		return;
	}

	vkDestroyInstance(this->instance, nullptr);
}

// API Accessibility
VkInstance vrender::render::Instance::get_handle() const
{
	return this->instance;
}

// Utility
void vrender::render::Instance::check_extension_list_support(const std::vector<std::string> extensions)
{
	uint32_t count;
	std::vector<VkExtensionProperties> platform_extensions;
	VkResult enumeration_result = vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
	if (enumeration_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Instance Creation Failed to Enumerate Extension Properties in First Phase");
	}

	platform_extensions.resize(count);
	enumeration_result = vkEnumerateInstanceExtensionProperties(nullptr, &count, platform_extensions.data());
	if (enumeration_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Instance Creation Failed to Enumerate Extension Properties in Second Phase");
	}

	std::vector<const char*> extension_names;
	extension_names.reserve(count);
	for (const VkExtensionProperties& extension_properties : platform_extensions)
	{
		extension_names.push_back(extension_properties.extensionName);
	}

	for (std::string requested_extension : extensions)
	{
		bool extension_present = false;
		for (const char* extension_name : extension_names)
		{
			if (strcmp(requested_extension.c_str(), extension_name) == 0)
			{
				extension_present = true;
				break;
			}
		}

		if (!extension_present)
		{
			throw std::runtime_error("ERROR: Vulkan Instance Creation Requested Invalid Extension: " + std::string(requested_extension));
		}
	}
}