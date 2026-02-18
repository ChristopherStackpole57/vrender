#ifndef RENDER_DESCRIPTOR_POOL_H
#define RENDER_DESCRIPTOR_POOL_H

#include <stdexcept>
#include <unordered_map>

#include <vulkan/vulkan.h>

#include <Core/LogicalDevice.h>
#include <Core/PipelineLayout.h>

namespace vrender::render
{
	class DescriptorPool
	{
	public:
		// Lifetime Control
		DescriptorPool(
			const vrender::render::LogicalDevice& logical_device,
			const std::vector<VkDescriptorPoolSize> pool_sizes,
			uint32_t max_sets
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