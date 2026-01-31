#include "Instance.h"

// Lifecycle Management
vrender::render::Instance::Instance(const vrender::render::config::InstanceConfig& config)
{
	std::vector<std::string> extensions = config.extensions;

	// Before creating the Vulkan instance create info, validate that the requested layers exist
	std::vector<const char*> layer_cstrs;
	if (config.enable_validation)
	{
		// If validation is enabled and a requested validation layer is missing, it is currently treated as a fatal issue
		// This decision prevents silently running without validation in debug
		check_layer_list_support(config.validation_layers);

		layer_cstrs.reserve(config.layers.size());
		for (const std::string& layer : config.layers)
		{
			layer_cstrs.push_back(layer.c_str());
		}

		// Once the requested layers have been shown as present, ensure that the DebugUtilsEXT is present
		bool found_debug_utils = false;
		for (const std::string& extension_name : config.extensions)
		{
			if (extension_name == "VK_EXT_debug_utils")
			{
				found_debug_utils = true;
				break;
			}
		}
		if (!found_debug_utils)
		{
			extensions.push_back(std::string("VK_EXT_debug_utils"));
		}
	}
	check_layer_list_support(config.layers);

	// Before creating the Vulkan instance create info, validate that the requested extensions exist.
	check_extension_list_support(extensions);

	std::vector<const char*> extension_cstrs;
	extension_cstrs.reserve(extensions.size());
	for (const std::string& extension : extensions)
	{
		extension_cstrs.push_back(extension.c_str());
	}

	// From Config create the application descriptor information
	VkApplicationInfo application_info{};
	application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	application_info.pNext = nullptr;
	application_info.pApplicationName = config.application_name.c_str();
	application_info.applicationVersion = config.application_version;
	application_info.pEngineName = config.engine_name.c_str();
	application_info.engineVersion = config.engine_version;
	application_info.apiVersion = config.api_version;

	// Create the 
	VkDebugUtilsMessengerCreateInfoEXT debug_messenger_info{};
	debug_messenger_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debug_messenger_info.pNext = nullptr;
	debug_messenger_info.flags = 0;
	debug_messenger_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debug_messenger_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
	debug_messenger_info.pfnUserCallback = &Instance::debug_messenger_callback;
	debug_messenger_info.pUserData = &debug_diagnostics;

	// From Config create the instance descriptor information
	VkInstanceCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pNext = nullptr;
	create_info.flags = 0;
	create_info.pApplicationInfo = &application_info;
	create_info.enabledLayerCount = static_cast<uint32_t>(layer_cstrs.size());
	create_info.ppEnabledLayerNames = layer_cstrs.data();
	create_info.enabledExtensionCount = static_cast<uint32_t>(extension_cstrs.size());
	create_info.ppEnabledExtensionNames = extension_cstrs.data();

	if (config.enable_validation)
	{
		create_info.pNext = &debug_messenger_info;
	}

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
		std::cout << "[Render] VRENDER Built Instance" << std::endl;
		std::cout << "[Render] Extensions Enabled: " << std::endl;
		for (const std::string& extension : extensions)
		{
			std::cout << " - " << extension << std::endl;
		}
	}

	if (config.enable_validation)
	{
		auto vkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
			vkGetInstanceProcAddr(this->instance, "vkCreateDebugUtilsMessengerEXT")
		);

		if (!vkCreateDebugUtilsMessengerEXT)
		{
			throw std::runtime_error("ERROR: Vulkan Failed to Load vkCreateDebugUtilsMessengerEXT");
		}

		VkResult debug_result = vkCreateDebugUtilsMessengerEXT(
			this->instance,
			&debug_messenger_info,
			nullptr,
			&(this->debug_messenger)
		);

		if (debug_result != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Vulkan Failed to Create Debug Messenger");
		}
	}
}
vrender::render::Instance::~Instance()
{
	if (this->debug_messenger != VK_NULL_HANDLE)
	{
		// There is a stored Debug Messenger that must be destroyed
		auto vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
			vkGetInstanceProcAddr(this->instance, "vkDestroyDebugUtilsMessengerEXT")
		);

		vkDestroyDebugUtilsMessengerEXT(
			this->instance,
			this->debug_messenger,
			nullptr
		);

		// In most cases unnecessary, but this is a defense against double destructor calls
		debug_messenger = VK_NULL_HANDLE;
	}

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
std::vector<VkPhysicalDevice> vrender::render::Instance::query_physical_devices() const
{
	uint32_t count = 0;
	std::vector<VkPhysicalDevice> devices;

	VkResult enumeration_result = vkEnumeratePhysicalDevices(this->instance, &count, nullptr);
	if (enumeration_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Instance Unable to Enumerate Physical Devices Phase One");
	}

	// Ensure that there are physical devices to query; eventually this will allow headless rendering
	if (count == 0)
	{
		throw std::runtime_error("ERROR: Vulkan Has No Physical Devices at Instance Enumeration");
	}

	devices.resize(count);
	enumeration_result = vkEnumeratePhysicalDevices(this->instance, &count, devices.data());
	if (enumeration_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Instance Unable to Enumerate Physical Devices Phase Two");
	}

	return devices;
}

// Utility
void vrender::render::Instance::check_layer_list_support(const std::vector<std::string>& layers)
{
	uint32_t count;
	std::vector<VkLayerProperties> platform_layers;
	VkResult enumeration_result = vkEnumerateInstanceLayerProperties(&count, nullptr);
	if (enumeration_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Instance Creation Failed to Enumerate Layer Properties in First Phase");
	}

	platform_layers.resize(count);
	enumeration_result = vkEnumerateInstanceLayerProperties(&count, platform_layers.data());
	if (enumeration_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Instance Creation Failed to Enumerate Layer Properties in Second Phase");
	}

	for (const std::string& requested_layer : layers)
	{
		bool layer_present = false;
		for (const VkLayerProperties& layer_properties : platform_layers)
		{
			if (strcmp(requested_layer.c_str(), layer_properties.layerName) == 0)
			{
				layer_present = true;
				break;
			}
		}

		if (!layer_present)
		{
			throw std::runtime_error("ERROR: Vulkan Instance Creation Requested Invalid Layer: " + requested_layer);
		}
	}
}
void vrender::render::Instance::check_extension_list_support(const std::vector<std::string>& extensions)
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

	for (const std::string& requested_extension : extensions)
	{
		bool extension_present = false;
		for (const VkExtensionProperties& extension_properties : platform_extensions)
		{
			if (strcmp(requested_extension.c_str(), extension_properties.extensionName) == 0)
			{
				extension_present = true;
				break;
			}
		}

		if (!extension_present)
		{
			throw std::runtime_error("ERROR: Vulkan Instance Creation Requested Invalid Extension: " + requested_extension);
		}
	}
}
VkBool32 vrender::render::Instance::debug_messenger_callback(
	VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
	VkDebugUtilsMessageSeverityFlagsEXT message_type,
	const VkDebugUtilsMessengerCallbackDataEXT* callback_data_ptr,
	void* user_data_ptr
)
{
	DebugDiagnostics* debug_diagnostics = static_cast<DebugDiagnostics*>(user_data_ptr);

	if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
	{
		// Vulkan encountered an error, this should be logged and mark a fatal flag
		std::cerr << "ERROR: Vulkan Encountered Error: " << std::endl << "\t" << callback_data_ptr->pMessage << std::endl;
		debug_diagnostics->validation_error_seen.store(true, std::memory_order_relaxed);
	}
	else if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	{
		// Vulkan encountered a warning, this should be logged only
		std::cerr << "WARNING: Vulkan Encountered Warning: " << std::endl << "\t" << callback_data_ptr->pMessage << std::endl;
	}

	return VK_FALSE;
}