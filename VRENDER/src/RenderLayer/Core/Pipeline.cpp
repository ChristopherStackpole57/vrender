#include <RenderLayer/Core/Pipeline.h>

// Lifetime Control
vrender::render::Pipeline::Pipeline(
	const vrender::render::LogicalDevice& logical_device,
	const vrender::render::config::PipelineConfiguration& config
)
	: logical_device_ptr(&logical_device)
	, bind_point(config.bind_point)
	, layout(&config.layout)

{
	// TODO: Switch on pipeline type
	std::vector<VkPipelineShaderStageCreateInfo> stages;
	stages.reserve(config.stages.size());
	for (const vrender::render::config::ShaderPipelineConfiguration& stage : config.stages)
	{
		stages.push_back(stage.get_vulkan_struct());
	}

	VkPipelineRenderingCreateInfo pipeline_rendering_create_info{};
	pipeline_rendering_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
	pipeline_rendering_create_info.pNext = nullptr;
	pipeline_rendering_create_info.colorAttachmentCount = static_cast<uint32_t>(config.attachment_formats.color_formats.size());
	pipeline_rendering_create_info.pColorAttachmentFormats = config.attachment_formats.color_formats.data();
	pipeline_rendering_create_info.depthAttachmentFormat = config.attachment_formats.depth_format;
	pipeline_rendering_create_info.stencilAttachmentFormat = config.attachment_formats.stencil_format;
	// pipeline_rendering_create_info rasterization samples

	VkGraphicsPipelineCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	create_info.pNext = &pipeline_rendering_create_info;
	create_info.flags = 0;
	create_info.stageCount = static_cast<uint32_t>(stages.size());
	create_info.pStages = stages.data();

	VkVertexInputBindingDescription binding{};
	binding.binding = 0;
	binding.stride = sizeof(Vertex);
	binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	VkVertexInputAttributeDescription attributes[] = {
		{
			.location = 0,
			.binding = 0,
			.format = VK_FORMAT_R32G32B32_SFLOAT,
			.offset = offsetof(Vertex, position)
		},
		{
			.location = 1,
			.binding = 0,
			.format = VK_FORMAT_R32G32B32_SFLOAT,
			.offset = offsetof(Vertex, color)
		}
	};

	VkPipelineVertexInputStateCreateInfo vertex_input{};
	vertex_input.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertex_input.vertexBindingDescriptionCount = 1;
	vertex_input.pVertexBindingDescriptions = &binding;
	vertex_input.vertexAttributeDescriptionCount = 2;
	vertex_input.pVertexAttributeDescriptions = attributes;

	create_info.pVertexInputState = &vertex_input;

	VkPipelineInputAssemblyStateCreateInfo input_assembly{};
	input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	input_assembly.primitiveRestartEnable = VK_FALSE;

	create_info.pInputAssemblyState = &input_assembly;

	//create_info.pTessellationState = ;

	VkPipelineViewportStateCreateInfo viewport_state{};
	viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_state.viewportCount = 1;
	viewport_state.pViewports = nullptr;
	viewport_state.scissorCount = 1;
	viewport_state.pScissors = nullptr;

	create_info.pViewportState = &viewport_state;
	
	VkPipelineRasterizationStateCreateInfo rasterizer_state{};
	rasterizer_state.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer_state.depthClampEnable = VK_FALSE;
	rasterizer_state.rasterizerDiscardEnable = VK_FALSE;
	rasterizer_state.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer_state.lineWidth = 1.0f;
	rasterizer_state.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer_state.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer_state.depthBiasEnable = VK_FALSE;

	create_info.pRasterizationState = &rasterizer_state;

	VkPipelineMultisampleStateCreateInfo multisampling_state{};
	multisampling_state.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling_state.sampleShadingEnable = VK_FALSE;
	multisampling_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	create_info.pMultisampleState = &multisampling_state;

	VkPipelineDepthStencilStateCreateInfo depth_stencil{};
	depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depth_stencil.pNext = nullptr;

	create_info.pDepthStencilState = &depth_stencil;
	
	VkPipelineColorBlendAttachmentState color_blend_attachment{};
	color_blend_attachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;
	color_blend_attachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo color_blending{};
	color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blending.logicOpEnable = VK_FALSE;
	//color_blending.attachmentCount = static_cast<uint32_t>(config.attachment_formats.color_formats.size());
	color_blending.attachmentCount = 1;
	color_blending.pAttachments = &color_blend_attachment;

	VkDynamicState dynamic_states[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo dynamic_state{};
	dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamic_state.pNext = nullptr;
	dynamic_state.dynamicStateCount = sizeof(dynamic_states) / sizeof(dynamic_states[0]);
	dynamic_state.pDynamicStates = dynamic_states;

	create_info.pColorBlendState = &color_blending;
	create_info.pDynamicState = &dynamic_state;
	create_info.layout = config.layout.get_layout();
	create_info.renderPass = nullptr;
	create_info.subpass = 0;

	VkResult creation_result = vkCreateGraphicsPipelines(
		logical_device.get_logical_device(),
		VK_NULL_HANDLE,
		1,
		&create_info,
		nullptr,
		&this->pipeline
	);
	if (creation_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: Vulkan Could Not Create Graphics Pipeline");
	}
}
vrender::render::Pipeline::~Pipeline()
{
	if (this->pipeline == VK_NULL_HANDLE || this->logical_device_ptr == nullptr)
	{
		return;
	}

	vkDestroyPipeline(
		this->logical_device_ptr->get_logical_device(),
		this->pipeline,
		nullptr
	);

	this->pipeline = VK_NULL_HANDLE;
	this->logical_device_ptr = nullptr;
}

vrender::render::Pipeline::Pipeline(vrender::render::Pipeline&& other) noexcept
	: pipeline(other.pipeline)
	, bind_point(other.bind_point)
	, logical_device_ptr(other.logical_device_ptr)
	, layout(other.layout)
{
	other.pipeline = VK_NULL_HANDLE;
	other.logical_device_ptr = nullptr;
	other.layout = nullptr;
}
vrender::render::Pipeline& vrender::render::Pipeline::operator=(vrender::render::Pipeline&& other) noexcept
{
	if (this != &other)
	{
		this->pipeline = other.pipeline;
		this->bind_point = other.bind_point;
		this->logical_device_ptr = other.logical_device_ptr;
		this->layout = other.layout;

		other.pipeline = VK_NULL_HANDLE;
		other.logical_device_ptr = nullptr;
		other.layout = nullptr;
	}

	return *this;
}

// API Accessibility
VkPipeline vrender::render::Pipeline::get_pipeline() const
{
	return this->pipeline;
}
const vrender::render::PipelineLayout* vrender::render::Pipeline::get_layout() const
{
	return this->layout;
}
VkPipelineBindPoint vrender::render::Pipeline::get_bind_point() const
{
	return this->bind_point;
}