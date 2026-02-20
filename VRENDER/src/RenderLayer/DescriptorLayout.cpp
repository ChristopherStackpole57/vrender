#include <Core/DescriptorLayout.h>

// Lifetime Control
vrender::render::DescriptorLayout::DescriptorLayout(
	const vrender::render::LogicalDevice& logical_device,
	const std::vector<vrender::render::config::BindingConfiguration>& bindings
)
	: logical_device_ptr(&logical_device)
{
	std::vector<VkDescriptorSetLayoutBinding> descriptor_bindings;
	descriptor_bindings.reserve(bindings.size());
	for (const vrender::render::config::BindingConfiguration& config : bindings)
	{
		descriptor_bindings.push_back(config.get_binding_config());
	}
	this->bindings = descriptor_bindings;

	// Bindless Flags
	std::vector<VkDescriptorBindingFlags> binding_flags;
	binding_flags.resize(bindings.size());
	for (int i = 0; i < bindings.size(); i++)
	{
		binding_flags[i] =
			VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT |
			VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT |
			VK_DESCRIPTOR_BINDING_UPDATE_UNUSED_WHILE_PENDING_BIT;
	}

	VkDescriptorSetLayoutBindingFlagsCreateInfo binding_flags_info{};
	binding_flags_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
	binding_flags_info.pNext = nullptr;
	binding_flags_info.bindingCount = static_cast<uint32_t>(binding_flags.size());
	binding_flags_info.pBindingFlags = binding_flags.data();

	VkDescriptorSetLayoutCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	create_info.pNext = &binding_flags_info;
	create_info.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;
	create_info.bindingCount = descriptor_bindings.size();
	create_info.pBindings = descriptor_bindings.data();

	VkResult creation_result = vkCreateDescriptorSetLayout(
		logical_device.get_logical_device(),
		&create_info,
		nullptr,
		&this->descriptor_layout
	);
	if (creation_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Was Unable to Create Descriptor Set Layout");
	}
}
vrender::render::DescriptorLayout::~DescriptorLayout()
{
	if (this->descriptor_layout == VK_NULL_HANDLE)
	{
		return;
	}

	vkDestroyDescriptorSetLayout(
		this->logical_device_ptr->get_logical_device(),
		this->descriptor_layout,
		nullptr
	);

	this->descriptor_layout = VK_NULL_HANDLE;
	this->logical_device_ptr = nullptr;
}

vrender::render::DescriptorLayout::DescriptorLayout(vrender::render::DescriptorLayout&& other) noexcept
	: descriptor_layout(other.descriptor_layout)
	, logical_device_ptr(other.logical_device_ptr)
{
	other.descriptor_layout = VK_NULL_HANDLE;
	other.logical_device_ptr = nullptr;
}
vrender::render::DescriptorLayout& vrender::render::DescriptorLayout::operator=(vrender::render::DescriptorLayout&& other) noexcept
{
	if (this != &other)
	{
		this->descriptor_layout = other.descriptor_layout;
		this->logical_device_ptr = other.logical_device_ptr;

		other.descriptor_layout = VK_NULL_HANDLE;
		other.logical_device_ptr = nullptr;
	}

	return *this;
}

// API Accessibility
VkDescriptorSetLayout vrender::render::DescriptorLayout::get_descriptor_layout() const
{
	return this->descriptor_layout;
}
std::vector<VkDescriptorSetLayoutBinding> vrender::render::DescriptorLayout::get_bindings() const
{
	return this->bindings;
}