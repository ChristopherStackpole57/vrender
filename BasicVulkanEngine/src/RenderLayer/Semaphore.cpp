#include "Semaphore.h"

// Lifetime Control
vrender::render::Semaphore::Semaphore(
	const vrender::render::LogicalDevice& logical_device
)
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

	this->device_ptr = &logical_device;
}
vrender::render::Semaphore::~Semaphore()
{
	if (this->semaphore == VK_NULL_HANDLE)
	{
		return;
	}

	vkDestroySemaphore(
		this->device_ptr->get_logical_device(),
		this->semaphore,
		nullptr
	);
}

// API Acessibility
const VkSemaphore vrender::render::Semaphore::get_semaphore() const
{
	return this->semaphore;
}