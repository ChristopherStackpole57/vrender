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
	const VkSurfaceKHR surface,
	const std::vector<std::string>& required_extensions
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
				.required = required_extensions
			},
			.surface{surface_requirements}
		}
	);

	std::cout << "[Render] VRENDER Selected Physical Device" << std::endl;

	return best_device;
}
static vrender::render::LogicalDevice build_logical_device(
	const vrender::render::PhysicalDevice& physical_device,
	const VkSurfaceKHR surface,
	const std::vector<std::string>& required_extensions
)
{
	vrender::render::utility::queue::QueueSelection queue_selection = vrender::render::utility::queue::select_queue_families(physical_device, surface);
	vrender::render::LogicalDevice logical_device(
		physical_device,
		queue_selection,
		required_extensions
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
		
	});

	return layouts;
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
	, MAX_FRAMES_IN_FLIGHT(max_frames)


	// Core
	, instance(build_instance_config(surface_provider, instance_config))
	, surface(surface_provider.create_surface(instance.get_instance()))
	, physical_device(build_physical_device(instance, surface, this->required_extensions))
	, logical_device(build_logical_device(physical_device, surface, this->required_extensions))
	, swapchain(build_swapchain(physical_device, logical_device, window_provider, surface))



	// Memory
	, allocator(instance, physical_device, logical_device)
	, geometry_arena(allocator, MAX_FRAMES_IN_FLIGHT)



	// Render Specific
	, frame_contexts(build_frame_contexts(logical_device, max_frames))

	, vertex(build_shader(logical_device, "buffer_vert.spv"))
	, fragment(build_shader(logical_device, "geo_frag.spv"))

	, descriptor_layouts(build_descriptor_layouts(logical_device))
	, persistent_descriptor_pool(
		logical_device,
		descriptor_controller->get_pool_sizes().pool_sizes,
		descriptor_controller->get_pool_sizes().max_sets
	)
	//, descriptor_controller()

	, pipeline_layout(
		logical_device,
		descriptor_layouts,
		push_constants
	)
	, pipeline(
		logical_device,
		vrender::render::config::PipelineConfiguration{
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipeline_layout,
			vrender::render::config::AttachmentFormats{
			
			},
			std::vector<vrender::render::config::ShaderPipelineConfiguration>{
				vrender::render::config::ShaderPipelineConfiguration{
					vertex,
					vrender::render::config::VERTEX_STAGE,
					"main"
				},
				vrender::render::config::ShaderPipelineConfiguration{
					fragment,
					vrender::render::config::FRAGMENT_STAGE,
					"main"
				}
			}
		}
	)
	
	, command_recorder(std::make_unique<vrender::render::DynamicCommandRecorder>(
		
	))

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
{
	// TODO: Clearly document static build function
}
vrender::render::Renderer::~Renderer()
{
	std::cout << "[Render] VRENDER Closing..." << std::endl;
	this->frame_contexts.clear();
	this->frame_targets.clear();

	this->command_recorder.release();

	this->command_controller.~CommandController();
	this->vertex.~Shader();
	this->fragment.~Shader();
	this->pipeline_layout.~PipelineLayout();
	this->pipeline.~Pipeline();
	this->swapchain.~Swapchain();
	this->logical_device.~LogicalDevice();
	this->physical_device.~PhysicalDevice();
	this->instance.~Instance();
	std::cout << "[Render] VRENDER Finished Closing" << std::endl;
}

// Public API
bool vrender::render::Renderer::step()
{
	// Wait for Completion of Current Frame's Fence Before Starting Next Frame
	VkFence wait_fence = this->frame_contexts[this->current_frame].in_flight.get_fence();
	vkWaitForFences(
		this->logical_device.get_logical_device(),
		1,
		&wait_fence,
		VK_TRUE,
		UINT64_MAX
	);
	vkResetFences(
		this->logical_device.get_logical_device(),
		1,
		&wait_fence
	);

	this->geometry_arena.reset_dynamic(current_frame);

	// Handle Window Resize
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

	// Acquire Swapchain Image and Validate
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

	// Write Pending Dynamic Frame Data
	std::vector<vrender::render::Mesh> frame_meshes;
	if (this->pending_dynamic_indices.size() > 0)
	{
		frame_meshes.insert(
			frame_meshes.end(),
			this->meshes.begin(),
			this->meshes.end()
		);
		for (size_t i = 0; i < this->pending_dynamic_vertices.size(); i++)
		{
			frame_meshes.push_back(this->geometry_arena.create_dynamic_mesh(
				this->pending_dynamic_vertices[i],
				this->pending_dynamic_indices[i],
				this->current_frame
			));
		}

		this->pending_dynamic_vertices.clear();
		this->pending_dynamic_indices.clear();
	}
	else
	{
		frame_meshes = this->meshes;
	}

	// Execute Frame
	uint32_t image = image_result.image_index;
	this->command_controller.record(
		this->current_frame,
		this->frame_contexts[this->current_frame],
		vrender::render::FrameDescriptorInputs{
			
		},
		frame_meshes
	);
	this->command_controller.submit(
		this->current_frame,
		this->frame_contexts[this->current_frame]
	);
	this->command_controller.present(
		image,
		this->frame_contexts[this->current_frame]
	);

	this->current_frame = (this->current_frame + 1) % this->frame_contexts.size();

	return true;
}

vrender::render::GeometryArena& vrender::render::Renderer::get_geometry_arena()
{
	return this->geometry_arena;
}
void vrender::render::Renderer::add_mesh(vrender::render::Mesh mesh)
{
	this->meshes.push_back(mesh);
}
void vrender::render::Renderer::render_dynamic_mesh(std::vector<vrender::render::Vertex> vertices, std::vector<uint32_t> indices)
{
	// TODO: Validate

	this->pending_dynamic_vertices.push_back(vertices);
	this->pending_dynamic_indices.push_back(indices);
}