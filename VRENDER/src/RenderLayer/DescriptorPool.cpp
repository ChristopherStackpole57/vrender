#include <Core/DescriptorPool.h>

// Lifetime Control
vrender::render::DescriptorPool::DescriptorPool(
	const vrender::render::LogicalDevice& logical_device,
	const std::vector<VkDescriptorPoolSize> pool_sizes,
	uint32_t max_sets,
	VkDescriptorPoolCreateFlags flags
)
	: logical_device_ptr(&logical_device)
{
	VkDescriptorPoolCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	create_info.pNext = nullptr;
	create_info.flags = flags;
	create_info.maxSets = max_sets;
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
		throw std::runtime_error("ERROR: Vulkan Failed to Create Descriptor Pool");
	}
}
vrender::render::DescriptorPool::~DescriptorPool()
{
	if (this->descriptor_pool == VK_NULL_HANDLE)
	{
		return;
	}

	vkDestroyDescriptorPool(
		this->logical_device_ptr->get_logical_device(),
		this->descriptor_pool,
		nullptr
	);
	this->descriptor_pool = VK_NULL_HANDLE;
}

vrender::render::DescriptorPool::DescriptorPool(vrender::render::DescriptorPool&& other) noexcept
	: descriptor_pool(other.descriptor_pool)
	, logical_device_ptr(other.logical_device_ptr)
{
	other.descriptor_pool = VK_NULL_HANDLE;
}
vrender::render::DescriptorPool& vrender::render::DescriptorPool::operator=(vrender::render::DescriptorPool&& other) noexcept
{
	if (this != &other)
	{
		if (this->descriptor_pool != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorPool(
				this->logical_device_ptr->get_logical_device(),
				this->descriptor_pool,
				nullptr
			);
		}

		this->descriptor_pool = other.descriptor_pool;
		this->logical_device_ptr = other.logical_device_ptr;

		other.descriptor_pool = VK_NULL_HANDLE;
	}

	return *this;
}

// API Accessibility
VkDescriptorPool vrender::render::DescriptorPool::get_descriptor_pool() const
{
	return this->descriptor_pool;
}