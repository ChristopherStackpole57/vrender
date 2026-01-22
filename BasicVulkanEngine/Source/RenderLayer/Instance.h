#ifndef RENDER_INSTANCE_H
#define RENDER_INSTANCE_H

#include <cstring>
#include <iostream>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "InstanceConfiguration.h"

namespace vrender
{
	namespace render
	{
		class Instance
		{
		public:
			// Lifecycle Management
			Instance(const vrender::render::InstanceConfig& config);
			~Instance();
			
			// Remove the Copy Constructor
			Instance(const Instance&) = delete;
			Instance& operator=(const Instance&) = delete;

			// API Accessibility
			VkInstance get_handle() const;

		private:
			VkInstance instance = VK_NULL_HANDLE;

			// Utility Methods
			void check_extension_list_support(const std::vector<std::string> extensions);
		};
	};
};

#endif