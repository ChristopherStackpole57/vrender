#ifndef RENDER_CONFIG_BINDING_H
#define RENDER_CONFIG_BINDING_H

#include <vulkan/vulkan.h>

namespace vrender::render::config
{
	struct BindingConfiguration
	{
		uint32_t index;
		VkDescriptorType type;
		uint32_t count;
		VkShaderStageFlags flags;

		BindingConfiguration(
			uint32_t binding_index,
			VkDescriptorType descriptor_type,
			uint32_t descriptor_count,
			VkShaderStageFlags stage_flags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT
		)
			: index(binding_index)
			, type(descriptor_type)
			, count(descriptor_count)
			, flags(stage_flags)
		{};

		VkDescriptorSetLayoutBinding get_binding_config() const
		{
			VkDescriptorSetLayoutBinding binding{};
			binding.binding = index;
			binding.descriptorType = type;
			binding.descriptorCount = count;
			binding.stageFlags = flags;
			binding.pImmutableSamplers = nullptr;

			return binding;
		}
	};
}

#endif