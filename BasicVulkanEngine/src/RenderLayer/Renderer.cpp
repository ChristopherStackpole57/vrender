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
	const vrender::platform::WindowProvider& window_provider,
	const vrender::platform::WindowSurfaceProvider& surface_provider, 
	const InstanceConfig& instance_config
) : instance(build_instance_config(surface_provider, instance_config))
{
	// TODO: Clear up what should be stored and not stored by the renderer

	// Create and bind a VkSurfaceKHR to use as a render target
	this->surface = surface_provider.create_surface(this->instance.get_handle());
	std::cout << "[Render] Vulkan Render Surface Created" << std::endl;

	// Query the instance for physical devices and choose the best one
	std::vector<vrender::render::PhysicalDevice> physical_devices = vrender::render::utility::physical_device::enumerate_physical_devices(this->instance);
	vrender::render::utility::physical_device::SurfaceRequirements surface_requirements{
		.surface = this->surface
	};
	vrender::render::PhysicalDevice best_device = vrender::render::utility::physical_device::select_physical_device(
		physical_devices,
		vrender::render::utility::physical_device::PhysicalDeviceSelectionParameters{
			.extensions{
				.required = std::vector<std::string>{ VK_KHR_SWAPCHAIN_EXTENSION_NAME }
			},
			.surface{surface_requirements}
		}
	);
	std::cout << "[Render] Vulkan Renderer Chose Ideal Physical Device: " << std::endl << "\t" << best_device.get_name() << std::endl;

	// Use the physical device to create a logical device
	vrender::render::utility::queue::QueueSelection queue_selection = vrender::render::utility::queue::select_queue_families(best_device, this->surface);
	vrender::render::LogicalDevice logical_device(
		best_device,
		queue_selection
	);
	std::cout << "[Render] Vulkan Renderer Created Logical Device From Physical Device" << std::endl;

	// Query swapchain support and create a swapchain
	vrender::render::utility::swapchain::SwapchainSupport swapchain_support = vrender::render::utility::swapchain::query_swapchain_support(
		best_device,
		this->surface
	);
	vrender::render::utility::swapchain::SwapchainConfiguration swapchain_configuration = vrender::render::utility::swapchain::configure_swapchain(
		swapchain_support,
		queue_selection,
		window_provider.get_framebuffer_size()
	);

	vrender::render::Swapchain swapchain(
		logical_device,
		this->surface,
		swapchain_configuration
	);
	std::cout << "[Render] Vulkan Renderer Created Swapchain" << std::endl;
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