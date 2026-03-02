#include <RenderLayer/Core/PipelineLayout.h>

// Lifetime Control
vrender::render::PipelineLayout::PipelineLayout(
	const vrender::render::LogicalDevice& logical_device,
	const std::vector<vrender::render::DescriptorLayout>& descriptor_layouts,
	const std::vector<VkPushConstantRange>& push_constants
)
	: logical_device_ptr(&logical_device)
	, descriptor_layouts(&descriptor_layouts)
{
	std::vector<VkDescriptorSetLayout> layouts;
	layouts.reserve(descriptor_layouts.size());
	for (const vrender::render::DescriptorLayout& layout : descriptor_layouts)
	{
		layouts.push_back(layout.get_descriptor_layout());
	}
	this->descriptor_layout_handles = layouts;

	VkPipelineLayoutCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	create_info.pNext = nullptr;
	create_info.flags = 0;
	create_info.setLayoutCount = static_cast<uint32_t>(layouts.size());
	create_info.pSetLayouts = layouts.data();
	create_info.pushConstantRangeCount = static_cast<uint32_t>(push_constants.size());
	create_info.pPushConstantRanges = push_constants.data();

	VkResult creation_result = vkCreatePipelineLayout(
		logical_device.get_logical_device(),
		&create_info,
		nullptr,
		&this->layout
	);
	if (creation_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Could Not Create Pipeline Layout");
	}
}
vrender::render::PipelineLayout::~PipelineLayout()
{
	if (this->layout == VK_NULL_HANDLE || this->logical_device_ptr == nullptr)
	{
		return;
	}

	vkDestroyPipelineLayout(
		this->logical_device_ptr->get_logical_device(),
		this->layout,
		nullptr
	);

	this->layout = VK_NULL_HANDLE;
}

vrender::render::PipelineLayout::PipelineLayout(vrender::render::PipelineLayout&& other) noexcept
	: layout(other.layout)
	, descriptor_layout_handles(other.descriptor_layout_handles)
	, descriptor_layouts(other.descriptor_layouts)
	, logical_device_ptr(other.logical_device_ptr)
{
	other.layout = VK_NULL_HANDLE;
	other.descriptor_layout_handles = {};
	other.logical_device_ptr = nullptr;
}
vrender::render::PipelineLayout& vrender::render::PipelineLayout::operator=(vrender::render::PipelineLayout&& other) noexcept
{
	if (this != &other)
	{
		if (this->layout != VK_NULL_HANDLE && this->logical_device_ptr)
		{
			vkDestroyPipelineLayout(
				this->logical_device_ptr->get_logical_device(),
				this->layout,
				nullptr
			);
		}

		this->layout = other.layout;
		this->descriptor_layouts = other.descriptor_layouts;
		this->descriptor_layout_handles = other.descriptor_layout_handles;
		this->logical_device_ptr = other.logical_device_ptr;

		other.layout = VK_NULL_HANDLE;
		other.descriptor_layout_handles = {};
		other.logical_device_ptr = nullptr;
	}

	return *this;
}

// API Accessibility
VkPipelineLayout vrender::render::PipelineLayout::get_layout() const
{
	return this->layout;
}
const std::vector<vrender::render::DescriptorLayout>& vrender::render::PipelineLayout::get_descriptor_layouts() const
{
	return *this->descriptor_layouts;
}
const std::vector<VkDescriptorSetLayout> vrender::render::PipelineLayout::get_descriptor_layout_handles() const
{
	return this->descriptor_layout_handles;
}