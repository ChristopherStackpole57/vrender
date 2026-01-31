#include "Renderer.h"

// Utility Function
static vrender::render::config::InstanceConfig build_instance_config(
	const vrender::platform::WindowSurfaceProvider& surface_provider,
	const vrender::render::config::InstanceConfig& base_config
)
{
	vrender::render::config::InstanceConfig config = base_config;

	std::vector<std::string> required_extensions = surface_provider.get_required_instance_extensions();
	config.extensions.insert(
		config.extensions.end(),
		required_extensions.begin(),
		required_extensions.end()
	);

	return config;
}
static vrender::render::PhysicalDevice build_physical_device(
	const vrender::render::Instance& instance,
	const VkSurfaceKHR surface
)
{
	// Enumerate Physical Devices
	std::vector<vrender::render::PhysicalDevice> physical_devices = vrender::render::utility::physical_device::enumerate_physical_devices(instance);
	vrender::render::utility::physical_device::SurfaceRequirements surface_requirements{
		.surface = surface
	};

	// Choose the best physical device
	vrender::render::PhysicalDevice best_device = vrender::render::utility::physical_device::select_physical_device(
		physical_devices,
		vrender::render::utility::physical_device::PhysicalDeviceSelectionParameters{
			.extensions{
				.required = std::vector<std::string>{ VK_KHR_SWAPCHAIN_EXTENSION_NAME }
			},
			.surface{surface_requirements}
		}
	);

	std::cout << "[Render] VRENDER Selected Physical Device" << std::endl;

	return best_device;
}
static vrender::render::LogicalDevice build_logical_device(
	const vrender::render::PhysicalDevice& physical_device,
	const VkSurfaceKHR surface
)
{
	vrender::render::utility::queue::QueueSelection queue_selection = vrender::render::utility::queue::select_queue_families(physical_device, surface);
	vrender::render::LogicalDevice logical_device(
		physical_device,
		queue_selection
	);

	std::cout << "[RENDER] VRENDER Built Logical Device" << std::endl;

	return logical_device;
}
static vrender::render::Swapchain build_swapchain(
	const vrender::render::PhysicalDevice& physical_device,
	const vrender::render::LogicalDevice& logical_device,
	const vrender::platform::WindowProvider& window_provider,
	const VkSurfaceKHR surface
)
{
	vrender::render::utility::swapchain::SwapchainSupport swapchain_support = vrender::render::utility::swapchain::query_swapchain_support(
		physical_device,
		surface
	);
	vrender::render::utility::swapchain::SwapchainConfiguration swapchain_configuration = vrender::render::utility::swapchain::configure_swapchain(
		swapchain_support,
		vrender::render::utility::queue::select_queue_families(physical_device, surface),
		window_provider.get_framebuffer_size()
	);

	vrender::render::Swapchain swapchain(
		logical_device,
		surface,
		swapchain_configuration
	);

	std::cout << "[Render] VRENDER Built Swapchain" << std::endl;

	return swapchain;
}
static vrender::render::Semaphore build_semaphore(
	const vrender::render::LogicalDevice& logical_device
)
{
	vrender::render::Semaphore semaphore(
		logical_device
	);
	return semaphore;

	std::cout << "[Render] VRENDER Built Semaphore" << std::endl;
}

// Lifetime Control
vrender::render::Renderer::Renderer(
	const vrender::platform::WindowProvider& window_provider,
	const vrender::platform::WindowSurfaceProvider& surface_provider,
	const vrender::render::config::InstanceConfig& instance_config
)
	: instance(build_instance_config(surface_provider, instance_config))
	, surface(surface_provider.create_surface(instance.get_handle()))
	, physical_device(build_physical_device(instance, surface))
	, logical_device(build_logical_device(physical_device, surface))
	, swapchain(build_swapchain(physical_device, logical_device, window_provider, surface))
	, test_semaphore(build_semaphore(logical_device))
{
	// TODO: Clearly document static build function

	// Create Base Render Pass and Store
	this->render_passes.emplace_back(
		std::make_unique<vrender::render::RenderPass>(
			logical_device,
			vrender::render::misc::basic_render_pass_config(swapchain)
		)
	);
	std::cout << "[Render] VRENDER Built RenderPass" << std::endl;

	// Create framebuffers from render pass and swapchain image views
	for (const VkImageView image_view : swapchain.get_image_views())
	{
		this->framebuffers.emplace_back(
			std::make_unique<vrender::render::Framebuffer>(
				logical_device,
				*this->render_passes.back().get(),
				std::vector<VkImageView>{ image_view },
				swapchain.get_extent()
			)
		);
	}
	std::cout << "[Render] VRENDER Built " << this->framebuffers.size() << " Framebuffers" << std::endl;
}
vrender::render::Renderer::~Renderer()
{

}

// Public API
void vrender::render::Renderer::step(const vrender::platform::WindowProvider& window)
{
	std::cout << "[Render] VRENDER Executing Frame..." << std::endl;

	if (window.was_resized())
	{
		// Window Resized, need to update accordingly
		// fence based syncing
		// recreate the swapchain to match new size
	}

	vrender::render::AcquireSwapchainImageResult image_result = this->swapchain.acquire_image(
		this->test_semaphore,
		UINT64_MAX
	);
	std::cout << "[Render] VRENDER Acquired Swapchain Image" << std::endl;

	std::cout << "[Render] VRENDER Completed Frame" << std::endl;
}