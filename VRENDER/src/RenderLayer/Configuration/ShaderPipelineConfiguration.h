#ifndef RENDER_CONFIG_SHADER_PIPELINE_H
#define RENDER_CONFIG_SHADER_PIPELINE_H

#include <string>

#include <vulkan/vulkan.h>

#include <Core/Shader.h>

namespace vrender::render::config
{
	const VkShaderStageFlagBits VERTEX_STAGE = VK_SHADER_STAGE_VERTEX_BIT;
	const VkShaderStageFlagBits FRAGMENT_STAGE = VK_SHADER_STAGE_FRAGMENT_BIT;

	struct ShaderPipelineConfiguration
	{
	public:
		std::string shader_name;

		ShaderPipelineConfiguration(
			const vrender::render::Shader& shader,
			const VkShaderStageFlagBits shader_stage,
			const std::string& name
		)
			: shader_name(name)
		{

			shader_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shader_info.pNext = nullptr;
			shader_info.flags = 0;
			shader_info.stage = shader_stage;
			shader_info.module = shader.get_shader_module();
			shader_info.pName = shader_name.c_str();
			shader_info.pSpecializationInfo = nullptr;
		};

		VkPipelineShaderStageCreateInfo get_vulkan_struct() const
		{
			return shader_info;
		};

	private:
		VkPipelineShaderStageCreateInfo shader_info{};
	};
}

#endif