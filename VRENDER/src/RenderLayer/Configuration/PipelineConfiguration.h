#ifndef RENDER_PIPELINE_CONFIGURATION_H
#define RENDER_PIPELINE_CONFIGURATION_H

#include <vulkan/vulkan.h>

#include <Core/PipelineLayout.h>

#include <RenderLayer/Configuration/AttachmentFormats.h>
#include <RenderLayer/Configuration/ShaderPipelineConfiguration.h>

namespace vrender::render::config
{
	struct PipelineConfiguration
	{
		VkPipelineBindPoint bind_point;
		const vrender::render::PipelineLayout& layout;
		const vrender::render::config::AttachmentFormats attachment_formats;
		std::vector<vrender::render::config::ShaderPipelineConfiguration> stages;
	};
}

#endif