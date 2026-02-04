#ifndef RENDER_PIPELINE_CONFIGURATION_H
#define RENDER_PIPELINE_CONFIGURATION_H

#include <vulkan/vulkan.h>

#include <LegacyRender/PipelineLayout.h>
#include <LegacyRender/RenderPass.h>

#include <RenderLayer/Configuration/ShaderPipelineConfiguration.h>

namespace vrender::render::config
{
	struct PipelineConfiguration
	{
		VkPipelineBindPoint bind_point;
		VkExtent2D extent;

		const vrender::render::RenderPass& render_pass;
		const vrender::render::PipelineLayout& layout;
		std::vector<vrender::render::config::ShaderPipelineConfiguration> stages;
	};
}

#endif