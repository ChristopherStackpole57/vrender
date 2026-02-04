#ifdef VRENDER_HAS_SDL

	#ifndef PLATFORM_WBACK_SDL
	#define PLATFORM_WBACK_SDL

	#include "WindowProvider.h"
	#include "WindowSurfaceProvider.h"

	#include <SDL3/SDL.h>

	namespace vrender
	{
		namespace platform
		{
			class SDLWindowBackend final
				: public vrender::platform::WindowProvider
				, public vrender::platform::WindowSurfaceProvider
			{
				// Lifetime Control 
				SDLWindowBackend();
				~SDLWindowBackend();

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