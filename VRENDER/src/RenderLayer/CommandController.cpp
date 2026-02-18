#include <Core/CommandController.h>

// Lifetime Control
vrender::render::CommandController::CommandController(
	const vrender::render::LogicalDevice& logical_device,
	const uint32_t queue_family_index,
	const vrender::render::Swapchain& swapchain,
	const vrender::render::ICommandRecorder* command_recorder,
	const vrender::render::IDescriptorController* descriptor_controller,
	const uint32_t max_frames_in_flight
)
	: logical_device_ptr(&logical_device)
	, swapchain_ptr(&swapchain)
	, command_recorder(command_recorder)
	, descriptor_controller(descriptor_controller)
{
	// TODO: validate queue_family_index points to real family
	// TOOD: add dependency to framebuffers to ensure frames are not in flight

	// Create Command Pools
	this->command_pools.resize(max_frames_in_flight);
	for (uint32_t i = 0; i < max_frames_in_flight; i++)
	{
		VkCommandPoolCreateInfo create_info{};
		create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		create_info.pNext = nullptr;
		// TODO: FIX THESE FLAGS
		create_info.flags = 0;
		create_info.queueFamilyIndex = queue_family_index;
	
		VkResult creation_result = vkCreateCommandPool(
			logical_device.get_logical_device(),
			&create_info,
			nullptr,
			&this->command_pools[i]
		);
		if (creation_result != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Vulkan Was Unable To Created Command Pool");
		}
	}

	// Create Command Buffers
	this->command_buffers.resize(max_frames_in_flight);
	for (uint32_t i = 0; i < max_frames_in_flight; i++)
	{
		VkCommandBufferAllocateInfo allocate_info{};
		allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocate_info.pNext = nullptr;
		allocate_info.commandPool = this->command_pools[i];
		allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocate_info.commandBufferCount = 1;

		VkResult allocation_result = vkAllocateCommandBuffers(
			logical_device.get_logical_device(),
			&allocate_info,
			&this->command_buffers[i]
		);
		if (allocation_result != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Vulkan Was Unable To Created Command Buffer");
		}
	}

	// Create Descriptor Pools
	/*vrender::render::DescriptorPoolRequirements pool_requirements = descriptor_controller->get_pool_sizes();
	this->descriptor_pools.reserve(max_frames_in_flight);
	for (uint32_t i = 0; i < max_frames_in_flight; i++)
	{
		this->descriptor_pools.emplace_back(
			logical_device, 
			pool_requirements.pool_sizes,
			pool_requirements.max_sets
		);
	}*/
}
vrender::render::CommandController::~CommandController()
{
	if (this->command_buffers.empty() || this->command_pools.empty())
	{
		return;
	}

	// TODO: correct clearing
	for (int i = 0; i < this->command_buffers.size(); i++)
	{
		vkFreeCommandBuffers(
			this->logical_device_ptr->get_logical_device(),
			this->command_pools[i],
			1,
			&this->command_buffers[i]
		);

		vkDestroyCommandPool(
			this->logical_device_ptr->get_logical_device(),
			this->command_pools[i],
			nullptr
		);
	}
	this->command_buffers.clear();
	this->command_pools.clear();
}

vrender::render::CommandController::CommandController(vrender::render::CommandController&& other) noexcept
	: logical_device_ptr(other.logical_device_ptr)
	, swapchain_ptr(std::move(other.swapchain_ptr))
	, command_recorder(std::move(other.command_recorder))
	, descriptor_controller(std::move(other.descriptor_controller))
	, command_pools(std::move(other.command_pools))
	, command_buffers(std::move(other.command_buffers))
{
	other.command_buffers = {};
	other.command_pools = {};
}
vrender::render::CommandController& vrender::render::CommandController::operator=(
	vrender::render::CommandController&& other	
) noexcept
{
	if (this != &other)
	{
		this->logical_device_ptr = other.logical_device_ptr;
		this->swapchain_ptr = std::move(other.swapchain_ptr);
		this->command_recorder = std::move(other.command_recorder);
		this->descriptor_controller = std::move(other.descriptor_controller);
		this->command_pools = std::move(other.command_pools);
		this->command_buffers = std::move(other.command_buffers);
	
		other.command_buffers = {};
		other.command_pools = {};
	}

	return *this;
}

// API Accessibility
void vrender::render::CommandController::record(
	uint32_t frame_index,
	vrender::render::FrameContext& frame_context,
	vrender::render::FrameDescriptorInputs inputs,
	const vrender::render::config::FrameDescription& frame_description,
	std::vector<vrender::render::Mesh>& meshes
)
{
	if (frame_index >= this->command_pools.size())
	{
		throw std::runtime_error("ERROR: Requested Command Recording on Non-Existent Frame: " + std::to_string(frame_index));
	}

	const VkCommandPool pool = this->command_pools[frame_index];
	const VkCommandBuffer buffer = this->command_buffers[frame_index];
	//const VkDescriptorPool descriptor_pool = this->descriptor_pools[frame_index].get_descriptor_pool();

	// Reset Command Pool
	vkResetCommandPool(
		this->logical_device_ptr->get_logical_device(),
		pool,
		0
	);

	// Reset Descriptor Pool
	/*vkResetDescriptorPool(
		this->logical_device_ptr->get_logical_device(),
		descriptor_pool,
		0
	);*/

	// Acquire Descriptor Sets
	/*const vrender::render::FrameDescriptorSets frame_descriptor_sets = this->descriptor_controller->prepare_descriptors(
		descriptor_pool,
		inputs
	);
	std::vector<VkDescriptorSet> descriptor_sets;
	descriptor_sets.reserve(frame_descriptor_sets.global_descriptors.size() + frame_descriptor_sets.frame_descriptors.size());
	descriptor_sets.insert(
		descriptor_sets.end(),
		frame_descriptor_sets.global_descriptors.begin(),
		frame_descriptor_sets.global_descriptors.end()
	);
	descriptor_sets.insert(
		descriptor_sets.end(),
		frame_descriptor_sets.frame_descriptors.begin(),
		frame_descriptor_sets.frame_descriptors.end()
	);*/

	// Begin Command Buffer
	VkCommandBufferBeginInfo begin_info{};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.pNext = nullptr;
	begin_info.flags = 0;
	begin_info.pInheritanceInfo = nullptr;

	vkBeginCommandBuffer(
		buffer,
		&begin_info
	);

	// Run Commands
	command_recorder->begin(buffer, frame_description);
	//command_recorder->record(buffer, descriptor_sets, meshes);
	command_recorder->record(buffer, {}, meshes);
	command_recorder->end(buffer);
	
	// End Command Buffer
	vkEndCommandBuffer(buffer);
}
void vrender::render::CommandController::submit(
	uint32_t frame_index,
	vrender::render::FrameContext& frame_context
)
{
	const VkSemaphore image_avilable = frame_context.image_available.get_semaphore();
	const VkSemaphore render_finished = frame_context.render_finished.get_semaphore();
	const VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	VkSubmitInfo submit_info{};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.waitSemaphoreCount = 1;
	submit_info.pWaitSemaphores = &image_avilable;
	submit_info.pWaitDstStageMask = &wait_stage;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &this->command_buffers[frame_index];
	submit_info.signalSemaphoreCount = 1;
	submit_info.pSignalSemaphores = &render_finished;

	vkQueueSubmit(
		this->logical_device_ptr->get_graphics_queue(),
		1,
		&submit_info,
		frame_context.in_flight.get_fence()
	);
}
void vrender::render::CommandController::present(
	uint32_t image_index,
	vrender::render::FrameContext& frame_context
)
{
	VkSwapchainKHR swapchain_handle = this->swapchain_ptr->get_swapchain();
	const VkSemaphore render_finished = frame_context.render_finished.get_semaphore();

	VkPresentInfoKHR present_info{};
	present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present_info.waitSemaphoreCount = 1;
	present_info.pWaitSemaphores = &render_finished;
	present_info.swapchainCount = 1;
	present_info.pSwapchains = &swapchain_handle;
	present_info.pImageIndices = &image_index;

	vkQueuePresentKHR(
		this->logical_device_ptr->get_graphics_queue(),
		&present_info
	);
}