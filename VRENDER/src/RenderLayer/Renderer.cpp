#include <Core/Renderer.h>

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
static vrender::render::Shader build_shader(
	const vrender::render::LogicalDevice& logical_device,
	const std::string path
)
{
	return vrender::render::Shader(
		logical_device,
		path
	);
}
static std::vector<vrender::render::DescriptorLayout> build_descriptor_layouts(
	const vrender::render::LogicalDevice& logical_device
)
{
	std::vector<vrender::render::DescriptorLayout> layouts;

	layouts.emplace_back(logical_device, std::vector<vrender::render::config::BindingConfiguration>{
		{ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, vrender::render::config::VERTEX_STAGE }
	});

	return layouts;
}
static vrender::render::PipelineLayout build_pipeline_layout(
	const vrender::render::LogicalDevice& logical_device,
	const std::vector<vrender::render::DescriptorLayout>& descriptor_layouts,
	const std::vector<VkPushConstantRange>& push_constants
)
{
	return vrender::render::PipelineLayout(
		logical_device,
		descriptor_layouts,
		push_constants
	);
}
static vrender::render::Pipeline build_pipeline(
	const vrender::render::LogicalDevice& logical_device,
	const vrender::render::Swapchain& swapchain,
	const vrender::render::RenderPass& render_pass,
	const vrender::render::PipelineLayout& layout,
	const vrender::render::Shader& vertex,
	const vrender::render::Shader& fragment
)
{
	return vrender::render::Pipeline(
		logical_device,
		vrender::render::config::PipelineConfiguration{
			.bind_point = VK_PIPELINE_BIND_POINT_GRAPHICS,
			.extent = swapchain.get_extent(),

			.render_pass = render_pass,
			.layout = layout,
			.stages = {
				vrender::render::config::ShaderPipelineConfiguration{ vertex, vrender::render::config::VERTEX_STAGE, "main" },
				vrender::render::config::ShaderPipelineConfiguration{ fragment, vrender::render::config::FRAGMENT_STAGE, "main" }
			}
		}
	);
}
static vrender::render::CommandController frame_and_command_factory(
	const vrender::render::LogicalDevice& logical_device,
	const vrender::render::Swapchain& swapchain,
	const vrender::render::RenderPass& render_pass,
	std::vector<vrender::render::Framebuffer>& framebuffers,
	std::vector<std::unique_ptr<vrender::render::IFrameTarget>>& frame_targets,
	std::vector<const vrender::render::IFrameTarget*>& frame_targets_raw,
	vrender::render::ICommandRecorder* command_recorder,
	vrender::render::IDescriptorController* descriptor_controller
)
{
	// Create framebuffers from render pass and swapchain image views
	const std::vector<VkImageView> image_views = swapchain.get_image_views();
	framebuffers.reserve(image_views.size());
	for (const VkImageView image_view : image_views)
	{
		framebuffers.emplace_back(
			logical_device,
			render_pass,
			std::vector<VkImageView>{ image_view },
			swapchain.get_extent()
		);
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
		command_recorder,
		descriptor_controller,
		frame_targets_raw
	);

	return command_controller;
}
static std::vector<vrender::render::FrameContext> build_frame_contexts(
	const vrender::render::LogicalDevice& logical_device,
	const uint32_t frame_count
)
{
	std::vector<vrender::render::FrameContext> frame_contexts;
	frame_contexts.reserve(frame_count);

	for (int i = 0; i < frame_count; i++)
	{
		frame_contexts.push_back(logical_device);
	}

	return frame_contexts;
}



struct alignas(16) TriangleUBO
{
	float vertices[6][4];
	float colors[6][4];
};
TriangleUBO ubo = {
	{
		{ 0.0f, -0.5f, 0.0f, 1.0f },
		{ 0.5f, 0.0f, 0.0f, 1.0f },
		{ -0.5f, 0.0f, 0.0f, 1.0f },

		{ 0.5f,  0.0f, 0.0f, 1.0f },
		{ 0.0f,  0.5f, 0.0f, 1.0f },
		{ -0.5f, 0.0f, 0.0f, 1.0f },
	},
	{
		{ 0.f, 0.f, 0.f, 1.f },
		{ 0.f, 0.f, 1.f, 1.f },
		{ 0.f, 1.f, 0.f, 1.f },

		{ 0.f, 0.f, 1.f, 1.f },
		{ 0.f, 0.f, 0.f, 1.f },
		{ 0.f, 1.f, 0.f, 1.f }
	}
};



// Lifetime Control
vrender::render::Renderer::Renderer(
	const vrender::platform::WindowProvider& window_provider,
	const vrender::platform::WindowSurfaceProvider& surface_provider,
	const vrender::render::config::InstanceConfig& instance_config,
	const uint32_t max_frames
)
// Basic
	: window_provider(window_provider)
	, window_surface_provider(window_surface_provider)

	// Core
	, instance(build_instance_config(surface_provider, instance_config))
	, surface(surface_provider.create_surface(instance.get_instance()))
	, physical_device(build_physical_device(instance, surface))
	, logical_device(build_logical_device(physical_device, surface))
	, swapchain(build_swapchain(physical_device, logical_device, window_provider, surface))

	// Memory
	, allocator(instance, physical_device, logical_device)
	, geo_buffer(
		allocator,
		sizeof(TriangleUBO),
		vrender::render::memory::BufferUsageClass::UNIFORM,
		vrender::render::memory::BufferCPUAccess::WRITE_ONCE,
		vrender::render::memory::BufferLifetime::PERSISTENT
	)

	// Render Specific
	, render_pass(build_render_pass(logical_device, swapchain))

	, vertex(build_shader(logical_device, "geo_vert.spv"))
	, fragment(build_shader(logical_device, "geo_frag.spv"))

	, descriptor_layouts(build_descriptor_layouts(logical_device))
	, descriptor_controller(std::make_unique<vrender::render::RenderPassDescriptorController>(logical_device, descriptor_layouts))

	, pipeline_layout(build_pipeline_layout(logical_device, this->descriptor_layouts, this->push_constants))
	, pipeline(build_pipeline(logical_device, swapchain, render_pass, pipeline_layout, vertex, fragment))
	, persistent_descriptor_pool(
		logical_device, 
		descriptor_controller->get_pool_sizes().pool_sizes,
		descriptor_controller->get_pool_sizes().max_sets
	)
	, command_recorder(std::make_unique<vrender::render::RenderPassCommandRecorder>(
		logical_device,
		physical_device,
		pipeline
	))

	// Generic Render
	, command_controller(frame_and_command_factory(
		logical_device,
		swapchain,
		render_pass,
		framebuffers,
		frame_targets,
		frame_targets_raw,
		command_recorder.get(),
		descriptor_controller.get()
	))
	, MAX_FRAMES_IN_FLIGHT(max_frames)
	, frame_contexts(build_frame_contexts(logical_device, max_frames))
{
	// TODO: Clearly document static build function
	this->geo_buffer.write(
		&ubo,
		sizeof(ubo)
	);
}
vrender::render::Renderer::~Renderer()
{
	std::cout << "[Render] VRENDER Closing..." << std::endl;
	this->framebuffers.clear();
	this->frame_contexts.clear();
	this->frame_targets.clear();

	this->command_recorder.release();

	this->command_controller.~CommandController();
	this->vertex.~Shader();
	this->fragment.~Shader();
	this->pipeline_layout.~PipelineLayout();
	this->pipeline.~Pipeline();
	this->render_pass.~RenderPass();
	this->swapchain.~Swapchain();
	this->logical_device.~LogicalDevice();
	this->physical_device.~PhysicalDevice();
	this->instance.~Instance();
	std::cout << "[Render] VRENDER Finished Closing" << std::endl;
}

// Public API
bool vrender::render::Renderer::step()
{
	if (this->window_provider.was_resized())
	{
		std::cout << "[Render] VRENDER Regenerating Swapchain..." << std::endl;

		// Wait for Fences to Complete
		for (const vrender::render::FrameContext& frame_context: this->frame_contexts)
		{
			const VkFence& fence_ptr = frame_context.in_flight.get_fence();
			vkWaitForFences(
				this->logical_device.get_logical_device(),
				1,
				&fence_ptr,
				VK_TRUE,
				UINT64_MAX
			);
		}

		this->framebuffers.clear();
		this->frame_contexts.clear();
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
			this->frame_targets,
			this->frame_targets_raw,
			this->command_recorder.get(),
			this->descriptor_controller.get()
		);

		// Regenerate Frame Contexts
		this->frame_contexts = build_frame_contexts(this->logical_device, this->framebuffers.size());

		// TODO: rebuild descriptors

		std::cout << "[Render] VRENDER Successfully Rebuilt Swapchain" << std::endl;
	}

	vrender::render::AcquireSwapchainImageResult image_result = this->swapchain.acquire_image(
		this->frame_contexts[current_frame].image_available,
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
		image_result.image_index,
		vrender::render::FrameDescriptorInputs{
			.frame_ubo = this->geo_buffer.get_buffer()
		}
	);
	this->command_controller.submit(
		image_result.image_index,
		this->frame_contexts[this->current_frame]
	);
	this->command_controller.present(
		image_result.image_index,
		this->frame_contexts[this->current_frame]
	);

	this->current_frame = (this->current_frame + 1) % this->frame_contexts.size();

	return true;
}