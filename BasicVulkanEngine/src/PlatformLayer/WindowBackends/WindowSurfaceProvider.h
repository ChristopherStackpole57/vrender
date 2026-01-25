#ifndef PLATFORM_WBACK_WINDOW_SURFACE_PROVIDER
#define PLATFORM_WBACK_WINDOW_SURFACE_PROVIDER

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

namespace vrender
{
	namespace platform
	{
		class WindowSurfaceProvider
		{
		public:
			virtual ~WindowSurfaceProvider() = default;
			virtual std::vector<std::string> get_required_instance_extensions() const = 0;
			virtual VkSurfaceKHR create_surface(VkInstance instance) const = 0;
			virtual bool destroy_surface(VkInstance instance, VkSurfaceKHR surface) const = 0;
		private:
		};
	};
};

#endif