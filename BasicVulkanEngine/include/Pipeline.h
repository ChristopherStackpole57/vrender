#ifndef RENDER_PIPELINE_H
#define RENDER_PIPELINE_H

#include <vulkan/vulkan.h>

#include <DescriptorLayout.h>
#include <LogicalDevice.h>

#include <RenderLayer/Configuration/PipelineConfiguration.h>

namespace vrender::render
{
	struct PushConstantRangeInfo
	{
		VkShaderStageFlags stages;
		uint32_t offset;
		uint32_t size;
	};

	class Pipeline
	{
	public:
		// Lifetime Control
		Pipeline(
			const vrender::render::LogicalDevice& logical_device,
			const vrender::render::config::PipelineConfiguration& config
		);
		~Pipeline();

		Pipeline(const Pipeline&) = delete;
		Pipeline& operator=(const Pipeline&) = delete;

		Pipeline(Pipeline&& other) noexcept;
		Pipeline& operator=(Pipeline&& other) noexcept;

		// API Accessiblity
		VkPipeline get_pipeline() const;
		VkPipelineLayout get_layout() const;
		VkPipelineBindPoint get_bind_point() const;
		//const vrender::render::PushConstantRangeInfo get_push_constants() const;
		//const vrender::render::DescriptorLayout& get_descriptor_layout() const;
		// const vrender::render::RenderPass& get_render_pass() const;
	private:
		VkPipeline pipeline;
		//VkPipelineLayout layout;
		VkPipelineBindPoint bind_point;
		const LogicalDevice* logical_device_ptr;
	};
}

#endif