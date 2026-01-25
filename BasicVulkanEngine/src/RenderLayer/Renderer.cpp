#include "Renderer.h"

// Utility Function
vrender::render::InstanceConfig build_instance_config(
	const vrender::platform::WindowSurfaceProvider& surface_provider,
	const vrender::render::InstanceConfig& base_config
)
{
	vrender::render::InstanceConfig config = base_config;

	std::vector<std::string> required_extensions = surface_provider.get_required_instance_extensions();
	config.extensions.insert(
		config.extensions.end(),
		required_extensions.begin(),
		required_extensions.end()
	);

	return config;
}

// Lifetime Control
vrender::render::Renderer::Renderer(
	const vrender::platform::WindowSurfaceProvider& surface_provider, 
	const InstanceConfig& instance_config
) : instance(build_instance_config(surface_provider, instance_config))
{
	// Create and bind a VkSurfaceKHR to use as a render target
	this->surface = surface_provider.create_surface(instance.get_handle());
	std::cout << "[RENDER] Vulkan Render Surface Created" << std::endl;
}
vrender::render::Renderer::~Renderer()
{

}

// Public API
void vrender::render::Renderer::step(const vrender::platform::WindowProvider& window)
{
	if (window.was_resized())
	{
		// Window Resized, need to update accordingly
		// fence based syncing
		// recreate the swapchain to match new size
	}
}