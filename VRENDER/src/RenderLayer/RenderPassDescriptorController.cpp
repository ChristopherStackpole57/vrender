#include <LegacyRender/RenderPassDescriptorController.h>

// Lifetime Control
vrender::render::RenderPassDescriptorController::RenderPassDescriptorController(
	const vrender::render::LogicalDevice& logical_device,
	// globals
	const std::vector<vrender::render::DescriptorLayout>& frame_layouts
)
	: logical_device_ptr(&logical_device)
{
	std::unordered_map<VkDescriptorType, uint32_t> counts;

	// Obtain Descriptor Layouts
	// globals
	// TODO: globals

	this->frame_layouts.reserve(frame_layouts.size());
	for (const vrender::render::DescriptorLayout& frame_layout : frame_layouts)
	{
		this->frame_layouts.push_back(frame_layout.get_descriptor_layout());

		for (VkDescriptorSetLayoutBinding binding : frame_layout.get_bindings())
		{
			counts[binding.descriptorType] += binding.descriptorCount * this->MAX_OBJECTS_PER_FRAME;
		}
	}

	this->pool_sizes.reserve(counts.size());
	for (auto& [type, count] : counts)
	{
		this->pool_sizes.push_back(VkDescriptorPoolSize{
			.type = type,
			.descriptorCount = count
		});
	}
}

// API Accessibility
vrender::render::DescriptorPoolRequirements vrender::render::RenderPassDescriptorController::get_pool_sizes() const
{
	return {
		this->MAX_OBJECTS_PER_FRAME,
		this->pool_sizes
	};
}
vrender::render::FrameDescriptorSets vrender::render::RenderPassDescriptorController::prepare_descriptors(
	VkDescriptorPool descriptor_pool,
	vrender::render::FrameDescriptorInputs inputs
) const
{
	if (inputs.frame_ubo == VK_NULL_HANDLE && inputs.global_ubo == VK_NULL_HANDLE)
	{
		return {};
	}

	// Allocate Descriptor Sets
	VkDescriptorSetAllocateInfo allocate_info{};
	allocate_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocate_info.pNext = nullptr;
	allocate_info.descriptorPool = descriptor_pool;
	allocate_info.descriptorSetCount = static_cast<uint32_t>(this->frame_layouts.size());
	allocate_info.pSetLayouts = frame_layouts.data();

	std::vector<VkDescriptorSet> frame_descriptor_sets;
	frame_descriptor_sets.resize(this->frame_layouts.size());
	VkResult alloc_result = vkAllocateDescriptorSets(
		this->logical_device_ptr->get_logical_device(),
		&allocate_info,
		frame_descriptor_sets.data()
	);
	if (alloc_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Unabled to Allocate Descriptors");
	}

	// Write Global Descriptor Sets
	// TODO: handle globals

	// Write Frame Descriptor Sets
	VkDescriptorBufferInfo buffer_info{};
	buffer_info.buffer = inputs.frame_ubo;
	buffer_info.offset = 0;
	buffer_info.range = 0;

	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.dstSet = frame_descriptor_sets[0];
	write.dstBinding = 0;
	write.dstArrayElement = 0;
	write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	write.descriptorCount = 1;
	write.pBufferInfo = &buffer_info;

	vkUpdateDescriptorSets(
		this->logical_device_ptr->get_logical_device(),
		1,
		&write,
		0,
		nullptr
	);

	return vrender::render::FrameDescriptorSets{
		// globals
		.frame_descriptors = frame_descriptor_sets
	};
}