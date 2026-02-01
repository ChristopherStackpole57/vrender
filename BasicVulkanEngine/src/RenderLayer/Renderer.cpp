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
static vrender::render::RenderPass build_render_pass(
	const vrender::render::LogicalDevice& logical_device,
	const vrender::render::Swapchain& swapchain
)
{
	vrender::render::RenderPass render_pass(
		logical_device,
		vrender::render::misc::basic_render_pass_config(swapchain)
	);

	std::cout << "[Render] VRENDER Built RenderPass" << std::endl;

	return render_pass;
}
static vrender::render::CommandController frame_and_command_factory(
	const vrender::render::LogicalDevice& logical_device,
	const vrender::render::Swapchain& swapchain,
	const vrender::render::RenderPass& render_pass,
	std::vector<vrender::render::Framebuffer>& framebuffers,
	std::vector<vrender::render::Fence>& frame_fences,
	std::vector<std::unique_ptr<vrender::render::IFrameTarget>>& frame_targets,
	std::vector<const vrender::render::IFrameTarget*>& frame_targets_raw,
	vrender::render::ICommandRecorder* command_recorder
)
{
	// Create framebuffers from render pass and swapchain image views
	const std::vector<VkImageView> image_views = swapchain.get_image_views();
	framebuffers.reserve(image_views.size());
	frame_fences.reserve(image_views.size());
	for (const VkImageView image_view : image_views)
	{
		framebuffers.emplace_back(
			logical_device,
			render_pass,
			std::vector<VkImageView>{ image_view },
			swapchain.get_extent()
		);

		frame_fences.push_back(logical_device);
	}
	std::cout << "[Render] VRENDER Built " << framebuffers.size() << " Framebuffers" << std::endl;

	// Create frame targets from framebuffers
	frame_targets.reserve(framebuffers.size());
	frame_targets_raw.reserve(framebuffers.size());
	for (const vrender::render::Framebuffer& framebuffer : framebuffers)
	{
		frame_targets.emplace_back(std::make_unique<
			vrender::render::RenderPassFrameTarget
		>(
			framebuffer,
			render_pass
		));

		frame_targets_raw.push_back(
			frame_targets.back().get()
		);
	}
	std::cout << "[Render] VRENDER Built " << frame_targets.size() << " Frame Targets" << std::endl;

	// Create Command Controller
	vrender::render::CommandController command_controller(
		logical_device,
		logical_device.get_graphics_queue_family_index(),
		swapchain,
		*command_recorder,
		frame_targets_raw
	);

	return command_controller;
}

// Lifetime Control
vrender::render::Renderer::Renderer(
	const vrender::platform::WindowProvider& window_provider,
	const vrender::platform::WindowSurfaceProvider& surface_provider,
	const vrender::render::config::InstanceConfig& instance_config
)
	: window_provider(window_provider)
	, window_surface_provider(window_surface_provider)
	, instance(build_instance_config(surface_provider, instance_config))
	, surface(surface_provider.create_surface(instance.get_handle()))
	, physical_device(build_physical_device(instance, surface))
	, logical_device(build_logical_device(physical_device, surface))
	, swapchain(build_swapchain(physical_device, logical_device, window_provider, surface))
	, test_semaphore(build_semaphore(logical_device))
	, render_pass(build_render_pass(logical_device, swapchain))
	, command_recorder(std::make_unique<vrender::render::RenderPassCommandRecorder>())
	, command_controller(frame_and_command_factory(
		logical_device,
		swapchain,
		render_pass,
		framebuffers,
		frame_fences,
		frame_targets,
		frame_targets_raw,
		command_recorder.get()
	))
{
	// TODO: Clearly document static build function
}
vrender::render::Renderer::~Renderer()
{
	std::cout << test_semaphore.device_ptr;

	this->framebuffers.clear();
	this->frame_targets.clear();
	this->frame_fences.clear();

	this->command_recorder.release();

	this->test_semaphore.~Semaphore();
	this->render_pass.~RenderPass();
	this->swapchain.~Swapchain();
	this->logical_device.~LogicalDevice();
	this->physical_device.~PhysicalDevice();
	this->instance.~Instance();
}

// Public API
bool vrender::render::Renderer::step()
{
	if (this->window_provider.was_resized())
	{
		std::cout << "[Render] VRENDER Regenerating Swapchain..." << std::endl;

		// Wait for Fences to Complete
		for (const vrender::render::Fence& fence : this->frame_fences)
		{
			const VkFence& fence_ptr = fence.get_fence();
			vkWaitForFences(
				this->logical_device.get_logical_device(),
				1,
				&fence_ptr,
				VK_TRUE,
				UINT64_MAX
			);
		}

		this->framebuffers.clear();
		this->frame_fences.clear();
		this->frame_targets.clear();
		this->frame_targets_raw.clear();

		this->swapchain.~Swapchain();

		// Regenerate Swapchain
		this->swapchain = build_swapchain(
			this->physical_device,
			this->logical_device,
			this->window_provider,
			this->surface
		);

		// Use Factory to Rebuild Render Chain
		this->command_controller = frame_and_command_factory(
			this->logical_device,
			this->swapchain,
			this->render_pass,
			this->framebuffers,
			this->frame_fences,
			this->frame_targets,
			this->frame_targets_raw,
			this->command_recorder.get()
		);

		std::cout << "[Render] VRENDER Successfully Rebuilt Swapchain" << std::endl;
	}

	vrender::render::AcquireSwapchainImageResult image_result = this->swapchain.acquire_image(
		this->test_semaphore,
		UINT64_MAX
	);

	if (image_result.result != VK_SUCCESS && image_result.result != VK_ERROR_OUT_OF_DATE_KHR)
	{
		std::cout << image_result.result << std::endl;
		return false;
	}
	else if (image_result.result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		// Skip rendering this frame, next frame will tick swapchain recreation
		return true;
	}
	
	this->command_controller.record(
		image_result.image_index
	);
	this->command_controller.submit(
		image_result.image_index,
		this->frame_fences[image_result.image_index]
	);
	this->command_controller.present(
		image_result.image_index
	);

	return true;
}