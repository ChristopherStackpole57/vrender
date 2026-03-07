#include <RenderLayer/Core/Renderer.h>

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
		// Camera Data
		{ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT },
		// Object Data
		{ 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT },
		// Textures
		// { 2}
		// Material Data
		// { 3, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1024, VK_SHADER_STAGE_FRAGMENT_BIT },
		// Lights
		// { 4 }
		// Samplers
		// { 5 }
	});

	return layouts;
}
static vrender::render::DescriptorPool build_descriptor_pool(
	const vrender::render::LogicalDevice& logical_device,
	const std::vector<vrender::render::DescriptorLayout>& layouts
)
{
	std::unordered_map<VkDescriptorType, uint32_t> counts;

	for (const vrender::render::DescriptorLayout& layout : layouts)
	{
		for (VkDescriptorSetLayoutBinding binding : layout.get_bindings())
		{
			counts[binding.descriptorType] += binding.descriptorCount;
		}
	}

	std::vector<VkDescriptorPoolSize> pool_sizes;
	pool_sizes.reserve(counts.size());

	for (auto& [type, count] : counts)
	{
		pool_sizes.push_back(VkDescriptorPoolSize{
			.type = type,
			.descriptorCount = count
		});
	}

	return vrender::render::DescriptorPool(
		logical_device,
		pool_sizes,
		1,
		VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT | VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT
	);
}
static vrender::render::CommandController frame_and_command_factory(
	const vrender::render::LogicalDevice& logical_device,
	const vrender::render::Swapchain& swapchain,
	vrender::render::ICommandRecorder* command_recorder,
	vrender::render::IDescriptorController* descriptor_controller,
	const uint32_t max_frames_in_flight
)
{
	// Create Command Controller
	vrender::render::CommandController command_controller(
		logical_device,
		logical_device.get_graphics_queue_family_index(),
		swapchain,
		command_recorder,
		descriptor_controller,
		max_frames_in_flight
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

	, vertex(build_shader(logical_device, "three_d.spv"))
	, fragment(build_shader(logical_device, "geo_frag.spv"))

	, descriptor_layouts(build_descriptor_layouts(logical_device))
	, push_constants({
		VkPushConstantRange{
			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
			.offset = 0,
			.size = sizeof(uint32_t)
		}
	})
	, persistent_descriptor_pool(build_descriptor_pool(logical_device, descriptor_layouts))
	, bindless_registry(
		logical_device,
		persistent_descriptor_pool,
		descriptor_layouts
	)

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
				.color_formats = {
					swapchain.get_image_format()
				},
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
		pipeline,
		geometry_arena
	))

	, command_controller(frame_and_command_factory(
		logical_device,
		swapchain,
		command_recorder.get(),
		descriptor_controller.get(),
		max_frames
	))



	// Testing
	, test_transform_buffer(
		allocator,
		sizeof(ame::mat4f) * 20,
		vrender::render::memory::BufferUsageClass::STORAGE,
		vrender::render::memory::BufferCPUAccess::WRITE_ONCE,
		vrender::render::memory::BufferLifetime::PERSISTENT
	)
	, test_camera_buffer(
		allocator,
		2* sizeof(ame::mat4f),
		vrender::render::memory::BufferUsageClass::UNIFORM,
		vrender::render::memory::BufferCPUAccess::WRITE_ONCE,
		vrender::render::memory::BufferLifetime::PERSISTENT
	)
{
	// TODO: Clearly document static build function
	this->images_in_flight.resize(this->swapchain.get_images().size());

	ame::mat4 transform = ame::TRS(
		ame::vec3f{ 0.0f, -0.75f, -2.0f },
		ame::vec3f{ 0.0f, 0.0f, 0.0f },
		ame::vec3f{ 1.0f, 1.0f, 1.0f }
	);
	transform = transform.Transpose();

	this->test_transform_buffer.write(
		transform.data(),
		sizeof(transform),
		0
	);
	this->transform_buffer_token = this->bindless_registry.register_storage_buffer(
		this->test_transform_buffer,
		1
	);

	// Write Camera Data
	ame::mat4 view = ame::TRS(
		ame::vec3f{ 0.0f, 0.0f, 0.0f },
		ame::vec3f{ 0.0f, 0.0f, 0.0f },
		ame::vec3f{ 1.0f, 1.0f, 1.0f }
	);
	view.Inverse();
	view = view.Transpose();

	this->test_camera_buffer.write(
		view.data(),
		sizeof(view),
		0
	);

	VkExtent2D extent = this->swapchain.get_extent();
	ame::mat4f perspective = ame::Perspective(
		0.1f,
		100.f,
		extent.width / (float)extent.height,
		60.0f
	);
	perspective = perspective.Transpose();
	
	std::cout << "perspective:\n" << perspective << std::endl;

	this->test_camera_buffer.write(
		perspective.data(),
		sizeof(perspective),
		sizeof(view)
	);

	this->camera_buffer_token = this->bindless_registry.register_uniform_buffer(
		this->test_camera_buffer,
		0
	);
}
vrender::render::Renderer::~Renderer()
{
	std::cout << "[Render] VRENDER Closing..." << std::endl;
	
	// Wait for Device Idle
	vkDeviceWaitIdle(this->logical_device.get_logical_device());
	
	// Destroy Objects
	vkDestroySurfaceKHR(
		this->instance.get_instance(),
		this->surface,
		nullptr
	);

	std::cout << "[Render] VRENDER Finished Closing" << std::endl;
}

// Public API
bool vrender::render::Renderer::step(const vrender::render::FrameData& frame_data)
{
	uint32_t offset = 0;
	std::vector<vrender::render::Mesh> meshes;
	meshes.reserve(frame_data.objects.size());
	for (const vrender::render::RenderObject& object : frame_data.objects)
	{
		this->test_transform_buffer.write(
			object.transform.Transpose().data(),
			sizeof(object.transform),
			offset
		);

		offset += sizeof(object.transform);
		meshes.push_back(
			this->geometry_arena.get_mesh(object.mesh)
		);
	}

	this->bindless_registry.update_storage_buffer(this->transform_buffer_token);
	time++;





	//ame::mat4 transform = ame::TRS(
		//ame::vec3f{ 2.75f * (float)std::sin(this->time / 750.f), -0.75f, -5.0f },
		//ame::vec3f{ this->time / 250.0f, this->time / 500.0f, this->time / 1000.0f },
		//ame::vec3f{ 1.0f, 1.0f, 1.0f }
	//);

	// Wait for Completion of Current Frame's Fence Before Starting Next Frame
	VkFence wait_fence = this->frame_contexts[this->current_frame].in_flight.get_fence();
	vkWaitForFences(
		this->logical_device.get_logical_device(),
		1,
		&wait_fence,
		VK_TRUE,
		UINT64_MAX
	);

	this->geometry_arena.reset_dynamic(current_frame);

	// Handle Window Resize
	if (this->window_provider.was_resized())
	{
		this->regenerate_swapchain();
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

	// Wait for Image Fence to Complete
	if (this->images_in_flight[image_result.image_index] != VK_NULL_HANDLE)
	{
		vkWaitForFences(
			this->logical_device.get_logical_device(),
			1,
			&images_in_flight[image_result.image_index],
			VK_TRUE,
			UINT64_MAX
		);
	}
	this->images_in_flight[image_result.image_index] = this->frame_contexts[this->current_frame].in_flight.get_fence();

	vkResetFences(
		this->logical_device.get_logical_device(),
		1,
		&wait_fence
	);

	// Write Pending Dynamic Frame Data
	std::vector<vrender::render::Mesh> frame_meshes;
	if (this->pending_dynamic_indices.size() > 0)
	{
		frame_meshes.insert(
			frame_meshes.end(),
			meshes.begin(),
			meshes.end()
		);
		for (size_t i = 0; i < this->pending_dynamic_vertices.size(); i++)
		{
			vrender::render::MeshToken token = this->geometry_arena.create_dynamic_mesh(
				this->pending_dynamic_vertices[i],
				this->pending_dynamic_indices[i],
				this->current_frame
			);

			frame_meshes.emplace_back(this->geometry_arena.get_mesh(token));
		}

		this->pending_dynamic_vertices.clear();
		this->pending_dynamic_indices.clear();
	}
	else
	{
		frame_meshes = meshes;
	}

	// Execute Frame
	uint32_t image = image_result.image_index;
	const VkImage swapchain_image = this->swapchain.get_image(image);
	const VkImageView swapchain_image_view = this->swapchain.get_image_view(image);
	const VkExtent2D extent = this->swapchain.get_extent();

	std::vector<VkDescriptorSet> frame_descriptor_sets;
	frame_descriptor_sets.emplace_back(this->bindless_registry.get_descriptor_set());
	this->command_controller.record(
		this->current_frame,
		vrender::render::config::FrameDescription{
			.swapchain_image = swapchain_image,
			.swapchain_image_view = swapchain_image_view,
			.extent = extent
		},
		frame_descriptor_sets,
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
void vrender::render::Renderer::render_dynamic_mesh(std::vector<vrender::render::Vertex> vertices, std::vector<uint32_t> indices)
{
	if (vertices.size() == 0 || indices.size() == 0)
	{
		return;
	}

	this->pending_dynamic_vertices.push_back(vertices);
	this->pending_dynamic_indices.push_back(indices);
}

// Utility
void vrender::render::Renderer::regenerate_swapchain()
{
	std::cout << "[Render] VRENDER Regenerating Swapchain..." << std::endl;

	// Wait for Device to Idle
	vkDeviceWaitIdle(this->logical_device.get_logical_device());

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
		this->command_recorder.get(),
		this->descriptor_controller.get(),
		this->MAX_FRAMES_IN_FLIGHT
	);

	std::cout << "[Render] VRENDER Successfully Rebuilt Swapchain" << std::endl;
}