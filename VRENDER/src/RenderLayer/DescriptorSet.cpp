#include <RenderLayer/Core/DescriptorSet.h>

// Lifetime Control
vrender::render::DescriptorSet::DescriptorSet(
	const vrender::render::LogicalDevice& logical_device,
	const vrender::render::DescriptorPool& descriptor_pool,
	const std::vector<vrender::render::DescriptorLayout>& descriptor_layouts
)
	: logical_device_ptr(&logical_device)
	, descriptor_pool_ptr(&descriptor_pool)
{
	std::vector<VkDescriptorSetLayout> raw_layouts;
	raw_layouts.reserve(descriptor_layouts.size());
	for (const vrender::render::DescriptorLayout& layout : descriptor_layouts)
	{
		raw_layouts.push_back(layout.get_descriptor_layout());
	}

	VkDescriptorSetAllocateInfo alloc_info{};
	alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	alloc_info.descriptorPool = descriptor_pool.get_descriptor_pool();
	alloc_info.descriptorSetCount = static_cast<uint32_t>(descriptor_layouts.size());
	alloc_info.pSetLayouts = raw_layouts.data();

	vkAllocateDescriptorSets(
		logical_device.get_logical_device(),
		&alloc_info,
		&this->descriptor_set
	);
}
vrender::render::DescriptorSet::~DescriptorSet()
{
	if (this->descriptor_set == VK_NULL_HANDLE)
	{
		return;
	}

	vkFreeDescriptorSets(
		this->logical_device_ptr->get_logical_device(),
		this->descriptor_pool_ptr->get_descriptor_pool(),
		1,
		&this->descriptor_set
	);
}

vrender::render::DescriptorSet::DescriptorSet(vrender::render::DescriptorSet&& other) noexcept
	: descriptor_set(other.descriptor_set)
	, logical_device_ptr(other.logical_device_ptr)
	, descriptor_pool_ptr(other.descriptor_pool_ptr)
{
	other.descriptor_set = VK_NULL_HANDLE;
	other.logical_device_ptr = nullptr;
	other.descriptor_pool_ptr = nullptr;
}
vrender::render::DescriptorSet& vrender::render::DescriptorSet::operator=(vrender::render::DescriptorSet&& other) noexcept
{
	if (this != &other)
	{
		if (this->descriptor_set != VK_NULL_HANDLE)
		{
			vkFreeDescriptorSets(
				this->logical_device_ptr->get_logical_device(),
				this->descriptor_pool_ptr->get_descriptor_pool(),
				1,
				&this->descriptor_set
			);
		}

		this->descriptor_set = other.descriptor_set;
		this->logical_device_ptr = other.logical_device_ptr;
		this->descriptor_pool_ptr = other.descriptor_pool_ptr;

		other.descriptor_set = VK_NULL_HANDLE;
		other.logical_device_ptr = nullptr;
		other.descriptor_pool_ptr = nullptr;
	}

	return *this;
}

// API Accessibility
VkDescriptorSet vrender::render::DescriptorSet::get_descriptor_set() const
{
	return this->descriptor_set;
}