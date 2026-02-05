#ifndef RENDER_PIPELINE_LAYOUT_H
#define RENDER_PIPELINE_LAYOUT_H

#include <vector>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include <Core/LogicalDevice.h>

#include <LegacyRender/DescriptorLayout.h>

namespace vrender::render
{
	class PipelineLayout
	{
	public:
		// Lifetime Control
		PipelineLayout(
			const vrender::render::LogicalDevice& logical_device,
			const std::vector<vrender::render::DescriptorLayout>& set_layouts,
			const std::vector<VkPushConstantRange>& push_constants
		);
		~PipelineLayout();
		
		PipelineLayout(const PipelineLayout&) = delete;
		PipelineLayout& operator=(const PipelineLayout&) = delete;

		PipelineLayout(PipelineLayout&& other) noexcept;
		PipelineLayout& operator=(PipelineLayout&& other) noexcept;

		// API Accessibility
		VkPipelineLayout get_layout() const;
		std::vector<VkDescriptorSetLayout> get_descriptor_layouts() const;
	private:
		VkPipelineLayout layout;
		std::vector<VkDescriptorSetLayout> descriptor_layouts;
		const vrender::render::LogicalDevice* logical_device_ptr;
	};
}

#endif