#include <LegacyRender/DescriptorPool.h>

// Lifetime Control
vrender::render::DescriptorPool::DescriptorPool(
	const vrender::render::LogicalDevice& logical_device,
	const vrender::render::PipelineLayout& pipeline_layout
)
	: logical_device_ptr(&logical_device)
{
	std::vector<vrender::render::DescriptorLayout> set_layouts = pipeline_layout.get_descriptor_layouts();

	std::unordered_map<VkDescriptorType, uint32_t> counts;
	for (const vrender::render::DescriptorLayout& layout : set_layouts)
	{
		for (VkDescriptorSetLayoutBinding binding : layout.get_bindings())
		{
			counts[binding.descriptorType] += binding.descriptorCount * MAX_OBJECTS_PER_FRAME;
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

	VkDescriptorPoolCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	create_info.pNext = nullptr;
	create_info.flags = 0;
	create_info.maxSets = MAX_OBJECTS_PER_FRAME;
	create_info.poolSizeCount = static_cast<uint32_t>(pool_sizes.size());
	create_info.pPoolSizes = pool_sizes.data();

	VkResult creation_result = vkCreateDescriptorPool(
		logical_device.get_logical_device(),
		&create_info,
		nullptr,
		&this->descriptor_pool
	);
	if (creation_result != VK_SUCCESS)
	{
		throw std::runtime_error();
	}
}
vrender::render::DescriptorPool::~DescriptorPool()
{

}

vrender::render::DescriptorPool::DescriptorPool(vrender::render::DescriptorPool&& other) noexcept
{

}
vrender::render::DescriptorPool& vrender::render::DescriptorPool::operator=(vrender::render::DescriptorPool&& other) noexcept
{

}

// API Accessibility
VkDescriptorPool vrender::render::DescriptorPool::get_descriptor_pool() const
{
	return this->descriptor_pool;
}