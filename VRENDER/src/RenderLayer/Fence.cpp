#include <Core/Fence.h>

// Lifetime Control
vrender::render::Fence::Fence(const vrender::render::LogicalDevice& logical_device)
	: logical_device_ptr(&logical_device)
{
	VkFenceCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	create_info.pNext = nullptr;
	create_info.flags = 0;

	VkResult creation_result = vkCreateFence(
		logical_device.get_logical_device(),
		&create_info,
		nullptr,
		&this->fence
	);
	if (creation_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Was Unable to Create Fence");
	}
}
vrender::render::Fence::~Fence()
{
	if (this->fence == VK_NULL_HANDLE)
	{
		return;
	}

	vkDestroyFence(
		this->logical_device_ptr->get_logical_device(),
		this->fence,
		nullptr
	);
}

vrender::render::Fence::Fence(vrender::render::Fence&& other) noexcept
	: fence(other.fence)
	, logical_device_ptr(other.logical_device_ptr)
{
	other.fence = VK_NULL_HANDLE;
}
vrender::render::Fence& vrender::render::Fence::operator=(vrender::render::Fence&& other) noexcept
{
	if (this != &other)
	{
		this->fence = other.fence;
		this->logical_device_ptr = other.logical_device_ptr;

		other.fence = VK_NULL_HANDLE;
	}

	return *this;
}

// API Accessibility
const VkFence vrender::render::Fence::get_fence() const
{
	return this->fence;
}