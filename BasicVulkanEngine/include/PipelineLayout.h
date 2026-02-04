#ifndef RENDER_PIPELINE_LAYOUT_H
#define RENDER_PIPELINE_LAYOUT_H

#include <vector>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include <DescriptorLayout.h>
#include <LogicalDevice.h>

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
	private:
		VkPipelineLayout layout;
		const vrender::render::LogicalDevice* logical_device_ptr;
	};
}

#endif