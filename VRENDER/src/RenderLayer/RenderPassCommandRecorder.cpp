#include <LegacyRender/RenderPassCommandRecorder.h>

// Utility
static uint32_t find_visible_mem(
	const vrender::render::PhysicalDevice* physical_device_ptr,
	uint32_t type_bits
)
{
	VkPhysicalDeviceMemoryProperties mem_props;
	vkGetPhysicalDeviceMemoryProperties(physical_device_ptr->get_physical_device(), &mem_props);

	for (uint32_t i = 0; i < mem_props.memoryTypeCount; i++)
	{
		if ((type_bits & (1 << i)) &&
			(mem_props.memoryTypes[i].propertyFlags &
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT))
		{
			return i;
		}
	}

	throw std::runtime_error("no mem");
}

// Lifetime Control
vrender::render::RenderPassCommandRecorder::RenderPassCommandRecorder(
	const vrender::render::LogicalDevice& logical_device,
	const vrender::render::PhysicalDevice& physical_device,
	const vrender::render::Pipeline& pipeline
)
	: pipeline(pipeline)
	, logical_device_ptr(&logical_device)
	, physical_device_ptr(&physical_device)
{

}
vrender::render::RenderPassCommandRecorder::~RenderPassCommandRecorder()
{

}

// API Accessibility
void vrender::render::RenderPassCommandRecorder::record(
	const VkCommandBuffer command_buffer,
	const vrender::render::IFrameTarget& frame_target
) const
{
	// No code necessary yet,
	// This will eventually get filled with a pipeline, descriptor set, etc.

	struct alignas(16) TriangleUBO
	{
		float colors[3][4];
	};

	TriangleUBO ubo = {
		{
			{ 1.f, 1.f, 0.f, 1.f },
			{ 0.f, 1.f, 1.f, 1.f },
			{ 1.f, 0.f, 1.f, 1.f }
		}
	};

	// Bind Pipeline
	vkCmdBindPipeline(
		command_buffer,
		this->pipeline.get_bind_point(),
		this->pipeline.get_pipeline()
	);

	// Bind Descriptor Sets
	//		Acquire Descriptor Set Layouts
	std::vector<VkDescriptorSetLayout> descriptor_layouts = this->pipeline.get_layout()->get_descriptor_layouts();

	//		Create Descriptor Pool and Sets
	VkDescriptorPoolSize pool_size{};
	pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	pool_size.descriptorCount = 1;
	
	VkDescriptorPoolCreateInfo pool_info{};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.pNext = nullptr;
	pool_info.flags = 0;
	pool_info.maxSets = 1;
	pool_info.poolSizeCount = 1;
	pool_info.pPoolSizes = &pool_size;
	
	VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;
	VkResult pool_result = vkCreateDescriptorPool(
		this->logical_device_ptr->get_logical_device(),
		&pool_info,
		nullptr,
		&descriptor_pool
	);
	if (pool_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Unable to Create Descriptor Pool");
	}

	//		Allocate Descriptor Sets
	VkDescriptorSetAllocateInfo alloc_info{};
	alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	alloc_info.pNext = nullptr;
	alloc_info.descriptorPool = descriptor_pool;
	alloc_info.descriptorSetCount = static_cast<uint32_t>(descriptor_layouts.size());
	alloc_info.pSetLayouts = descriptor_layouts.data();

	std::vector<VkDescriptorSet> descriptor_sets;
	descriptor_sets.resize(descriptor_layouts.size());
	VkResult alloc_result = vkAllocateDescriptorSets(
		this->logical_device_ptr->get_logical_device(),
		&alloc_info,
		descriptor_sets.data()
	);
	if (alloc_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Unabled to Allocate Descriptors");
	}

	//		Create Buffer Data
	VkBufferCreateInfo buffer_create_info{};
	buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_create_info.size = sizeof(ubo);
	buffer_create_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkBuffer uniform_buffer;
	VkResult buffer_result = vkCreateBuffer(
		this->logical_device_ptr->get_logical_device(),
		&buffer_create_info,
		nullptr,
		&uniform_buffer
	);
	if (buffer_result)
	{
		throw std::runtime_error("ERROR: Vulkan Failed to Create Uniform Buffer");
	}

	VkMemoryRequirements mem_requirements;
	vkGetBufferMemoryRequirements(
		this->logical_device_ptr->get_logical_device(),
		uniform_buffer,
		&mem_requirements
	);

	VkMemoryAllocateInfo mem_allocate_info{};
	mem_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	mem_allocate_info.allocationSize = mem_requirements.size;
	mem_allocate_info.memoryTypeIndex = find_visible_mem(
		this->physical_device_ptr,
		mem_requirements.memoryTypeBits
	);

	VkDeviceMemory uniform_memory;
	VkResult mem_alloc_result = vkAllocateMemory(
		this->logical_device_ptr->get_logical_device(),
		&mem_allocate_info,
		nullptr,
		&uniform_memory
	);
	if (mem_alloc_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Failed to Allocate Uniform Buffer Memory");
	}

	vkBindBufferMemory(
		this->logical_device_ptr->get_logical_device(),
		uniform_buffer,
		uniform_memory,
		0
	);

	//		Populate Descriptors
	VkDescriptorBufferInfo buffer_info{};
	buffer_info.buffer = uniform_buffer;
	buffer_info.offset = 0;
	buffer_info.range = sizeof(TriangleUBO);

	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.dstSet = descriptor_sets[0];
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

	void* mapped;
	vkMapMemory(
		this->logical_device_ptr->get_logical_device(),
		uniform_memory,
		0,
		sizeof(TriangleUBO),
		0,
		&mapped
	);
	std::memcpy(mapped, &ubo, sizeof(TriangleUBO));
	vkUnmapMemory(this->logical_device_ptr->get_logical_device(), uniform_memory);

	//		Bind Sets
	vkCmdBindDescriptorSets(
		command_buffer,
		this->pipeline.get_bind_point(),
		this->pipeline.get_layout()->get_layout(),
		0,
		static_cast<uint32_t>(descriptor_sets.size()),
		descriptor_sets.data(),
		0,
		nullptr
	);

	// Push Constants
	//vkCmdPushConstants();

	// Draw
	vkCmdDraw(
		command_buffer,
		3,
		1,
		0,
		0
	);
}