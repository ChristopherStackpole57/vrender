#ifdef VRENDER_HAS_WIN32
	#ifndef PLATFORM_WBACK_WIN32_H
	#define PLATFORM_WBACK_WIN32_H

	#include "WindowProvider.h"
	#include "WindowSurfaceProvider.h"

	namespace vrender
	{
		namespace platform
		{
			class WIN32WindowSurfaceProvider : public vrender::platform::WindowSurfaceProvider
			{
				// Lifetime Control
				WIN32WindowBackend();
				~WIN32WindowBackend();

				// WindowProvider Contract

				// WindowSurfaceProvider Contract
				std::vector<std::string> get_required_instance_extensions() const override;
				VkSurfaceKHR create_surface(VkInstance instance) const override;
				bool destroy_surface(VkInstance instance, VkSurfaceKHR surface) const override;
			};
		};
	};

	#endif
#endif