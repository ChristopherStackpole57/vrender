#ifndef RENDER_PIPELINE_LAYOUT_H
#define RENDER_PIPELINE_LAYOUT_H

#include <vector>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include <Core/DescriptorLayout.h>
#include <Core/LogicalDevice.h>

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
		const std::vector<vrender::render::DescriptorLayout>& get_descriptor_layouts() const;
		const std::vector<VkDescriptorSetLayout> get_descriptor_layout_handles() const;
	private:
		VkPipelineLayout layout;
		std::vector<VkDescriptorSetLayout> descriptor_layout_handles;
		const vrender::render::LogicalDevice* logical_device_ptr;
		const std::vector<vrender::render::DescriptorLayout>* descriptor_layouts;
	};
}

#endif