#include "Renderer.h"

// Utility Function
static vrender::render::InstanceConfig build_instance_config(
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
	this->surface = surface_provider.create_surface(this->instance.get_handle());
	std::cout << "[Render] Vulkan Render Surface Created" << std::endl;

	// Query the instance for physical devices and choose the best one
	std::vector<vrender::render::PhysicalDevice> physical_devices = vrender::render::utility::physical_device::enumerate_physical_devices(this->instance);
	vrender::render::PhysicalDevice best_device = vrender::render::utility::physical_device::select_physical_device(
		physical_devices,
		vrender::render::utility::physical_device::PhysicalDeviceSelectionParameters{}
	);
	std::cout << "[Render] Vulkan Renderer Chose Ideal Physical Device: " << std::endl << "\t" << best_device.get_name() << std::endl;

	// Use the physical device to create a logical device
	vrender::render::LogicalDevice logical_device(
		best_device,
		vrender::render::utility::queue::select_queue_families(
			best_device,
			this->surface
		)
	);
	std::cout << "[Render] Vulkan Renderer Created Logical Device From Physical Device" << std::endl;
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