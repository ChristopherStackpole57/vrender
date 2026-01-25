#ifdef VRENDER_HAS_SFML

	#ifndef PLATFORM_WBACK_SFML_H
	#define PLATFORM_WBACK_SFML_H

	#include "WindowProvider.h"
	#include "WindowSurfaceProvider.h"

	namespace vrender
	{
		namespace platform
		{
			class SFMLWindowBackend final
				: public vrender::platform::WindowProvider
				, public vrender::platform::WindowSurfaceProvider
			{
				// Lifetime Control
				SFMLWindowBackend();
				~SFMLWindowBackend();

				// WindowProvider Contract

				// WindowSurfaceProvider Contract

			};
		};
	};

	#endif
#endif