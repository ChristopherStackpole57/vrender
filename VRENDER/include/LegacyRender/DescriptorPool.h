#ifndef RENDER_DESCRIPTOR_POOL_H
#define RENDER_DESCRIPTOR_POOL_H

#include <stdexcept>
#include <unordered_map>

#include <vulkan/vulkan.h>

#include <Core/LogicalDevice.h>
#include <LegacyRender/PipelineLayout.h>

namespace vrender::render
{
	uint16_t MAX_OBJECTS_PER_FRAME = 500;

	class DescriptorPool
	{
	public:
		// Lifetime Control
		DescriptorPool(
			const vrender::render::LogicalDevice& logical_device,
			const vrender::render::PipelineLayout& pipeline_layout
		);
		~DescriptorPool();

		DescriptorPool(const DescriptorPool&) = delete;
		DescriptorPool& operator=(const DescriptorPool&) = delete;

		DescriptorPool(DescriptorPool&& other) noexcept;
		DescriptorPool& operator=(DescriptorPool&& other) noexcept;

		// API Accessibility
		VkDescriptorPool get_descriptor_pool() const;
	private:
		VkDescriptorPool descriptor_pool;
		const vrender::render::LogicalDevice* logical_device_ptr;
	};
}

#endif