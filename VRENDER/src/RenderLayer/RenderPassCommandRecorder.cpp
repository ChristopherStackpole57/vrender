#include <LegacyRender/RenderPassCommandRecorder.h>

// Lifetime Control
vrender::render::RenderPassCommandRecorder::RenderPassCommandRecorder(
	const vrender::render::LogicalDevice& logical_device,
	const vrender::render::PhysicalDevice& physical_device,
	const vrender::render::Pipeline& pipeline,
	const vrender::render::memory::Buffer& geo_buffer
)
	: pipeline(pipeline)
	, logical_device_ptr(&logical_device)
	, physical_device_ptr(&physical_device)
	, geo_buffer(geo_buffer)
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
	// Bind Pipeline
	vkCmdBindPipeline(
		command_buffer,
		this->pipeline.get_bind_point(),
		this->pipeline.get_pipeline()
	);

	// plan for descriptors
	// each frame owns a descriptor pool, held in command controller(?)
	// descriptor set layouts are already set in the pipeline
	// per frame allocate sets for these layouts
	// per frame bind these sets
	// per frame write into these sets
	// besides per frame descriptor pools there are persistent descriptor pools
	// these contain things like materials, static textures, static buffers, etc
	// vs transient pools which store things like per draw ubos, constants, etc

	// Bind Descriptor Sets
	//		Acquire Descriptor Set Layouts
	const std::vector<VkDescriptorSetLayout> descriptor_layouts = this->pipeline.get_layout()->get_descriptor_layout_handles();
	uint32_t set_count = static_cast<uint32_t>(descriptor_layouts.size());

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

	VkDescriptorBufferInfo buffer_info{};
	buffer_info.buffer = this->geo_buffer.get_buffer();
	buffer_info.offset = 0;
	buffer_info.range = 16 * 6 * 2;

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
		6,
		1,
		0,
		0
	);
}