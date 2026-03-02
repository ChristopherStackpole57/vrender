#include <RenderLayer/Core/Semaphore.h>

// Lifetime Control
vrender::render::Semaphore::Semaphore(
	const vrender::render::LogicalDevice& logical_device
)
	: device_ptr(&logical_device)
{
	VkSemaphoreCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	create_info.pNext = nullptr;
	create_info.flags = 0;

	VkResult creation_result = vkCreateSemaphore(
		logical_device.get_logical_device(),
		&create_info,
		nullptr,
		&this->semaphore
	);
	if (creation_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Unable to Create Semaphore");
	}
}
vrender::render::Semaphore::~Semaphore()
{
	if (this->semaphore == VK_NULL_HANDLE || this->device_ptr == nullptr)
	{
		return;
	}

	vkDestroySemaphore(
		this->device_ptr->get_logical_device(),
		this->semaphore,
		nullptr
	);

	this->semaphore = VK_NULL_HANDLE;
	this->device_ptr = nullptr;
}

vrender::render::Semaphore::Semaphore(vrender::render::Semaphore&& other) noexcept
	: semaphore(other.semaphore)
	, device_ptr(other.device_ptr)
{
	other.semaphore = VK_NULL_HANDLE;
	other.device_ptr = nullptr;
}
vrender::render::Semaphore& vrender::render::Semaphore::operator=(vrender::render::Semaphore&& other) noexcept
{
	if (this != &other)
	{
		this->semaphore = other.semaphore;
		this->device_ptr = other.device_ptr;

		other.semaphore = VK_NULL_HANDLE;
		other.device_ptr = nullptr;
	}

	return *this;
}

// API Acessibility
const VkSemaphore vrender::render::Semaphore::get_semaphore() const
{
	return this->semaphore;
}