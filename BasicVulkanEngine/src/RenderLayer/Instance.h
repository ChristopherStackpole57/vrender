#ifndef RENDER_INSTANCE_H
#define RENDER_INSTANCE_H

#include <cstring>
#include <iostream>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "DebugDiagnostics.h"
#include "InstanceConfiguration.h"

namespace vrender
{
	namespace render
	{
		class Instance
		{
		public:
			// Lifecycle Management
			Instance() {};
			Instance(const vrender::render::InstanceConfig& config);
			~Instance();
			
			// Remove the Copy Constructor
			Instance(const Instance&) = delete;
			Instance& operator=(const Instance&) = delete;

			// API Accessibility
			VkInstance get_handle() const;

		private:
			VkInstance instance = VK_NULL_HANDLE;
			VkDebugUtilsMessengerEXT debug_messenger = VK_NULL_HANDLE;
			DebugDiagnostics debug_diagnostics;

			// Utility Methods
			void check_layer_list_support(const std::vector<std::string>& layers);
			void check_extension_list_support(const std::vector<std::string>& extensions);
		
			static VkBool32 VKAPI_CALL debug_messenger_callback(
				VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
				VkDebugUtilsMessageSeverityFlagsEXT message_type,
				const VkDebugUtilsMessengerCallbackDataEXT* callback_data_ptr,
				void* user_data_ptr
			);
		};
	};
};

#endif