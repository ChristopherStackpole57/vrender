#include <RenderCycle/BindlessRegistry.h>

// Lifetime Control
vrender::render::BindlessRegistry::BindlessRegistry(
	const vrender::render::LogicalDevice& logical_device,
	const vrender::render::DescriptorPool& descriptor_pool,
	const std::vector<vrender::render::DescriptorLayout>& descriptor_layouts
)
	: descriptor_set(logical_device, descriptor_pool, descriptor_layouts)
	, logical_device_ptr(&logical_device)
{
	for (const vrender::render::DescriptorLayout& layout : descriptor_layouts)
	{
		const std::vector<VkDescriptorSetLayoutBinding> bindings = layout.get_bindings();
		for (const VkDescriptorSetLayoutBinding& binding : bindings)
		{
			if (binding.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
			{
				this->storage_buffer_suballocators.emplace(
					binding.binding,
					vrender::render::memory::Suballocator(
						vrender::render::memory::SuballocatorStrategy::FREE_LIST,
						0,
						binding.descriptorCount
					)
				);
			}
		}
	}
}
vrender::render::BindlessRegistry::~BindlessRegistry()
{

}

// API Accessibility
uint32_t vrender::render::BindlessRegistry::register_storage_buffer(
	const vrender::render::memory::Buffer& buffer,
	uint32_t binding
)
{
	uint32_t index = this->storage_buffer_suballocators[0].allocate(1);
	if (index == UINT32_MAX)
	{
		throw std::runtime_error("ERROR: Bindless Buffer Capacity Exhausted");
	}

	VkDescriptorBufferInfo buffer_info{};
	buffer_info.buffer = buffer.get_buffer();
	buffer_info.offset = 0;
	buffer_info.range = buffer.get_size();

	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.dstSet = this->descriptor_set.get_descriptor_set();
	write.dstBinding = binding;
	write.dstArrayElement = index;
	write.descriptorCount = 1;
	write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	write.pBufferInfo = &buffer_info;

	vkUpdateDescriptorSets(
		this->logical_device_ptr->get_logical_device(),
		1,
		&write,
		0,
		nullptr
	);

	return index;
}
void vrender::render::BindlessRegistry::update_storage_buffer(uint32_t index)
{

}