#ifndef RENDER_PIPELINE_H
#define RENDER_PIPELINE_H

#include <vulkan/vulkan.h>

#include <Core/LogicalDevice.h>

#include <LegacyRender/DescriptorLayout.h>

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
		const vrender::render::PipelineLayout* get_layout() const;
		VkPipelineBindPoint get_bind_point() const;
	private:
		VkPipeline pipeline;
		VkPipelineBindPoint bind_point;
		const vrender::render::PipelineLayout* layout;
		const LogicalDevice* logical_device_ptr;
	};
}

#endif