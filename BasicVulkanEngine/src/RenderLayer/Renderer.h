#ifndef RENDER_RENDERER_H
#define RENDER_RENDERER_H

#include <string>

#include <PlatformLayer/WindowBackends/WindowProvider.h>
#include <PlatformLayer/WindowBackends/WindowSurfaceProvider.h>

#include <RenderLayer/InstanceConfiguration.h>
#include <RenderLayer/Instance.h>

namespace vrender
{
	namespace render
	{
		class Renderer
		{
		public:
			// Lifetime Control
			Renderer(
				const vrender::platform::WindowSurfaceProvider& surface_provider_ptr, 
				const vrender::render::InstanceConfig& instance_config
			);
			~Renderer();

			// Public API
			void step(const vrender::platform::WindowProvider& window);
		private:
			vrender::render::Instance instance;
			VkSurfaceKHR surface;
		};
	}
}

#endif