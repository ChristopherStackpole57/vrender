#ifndef RENDER_DESCRIPTOR_SET_H
#define RENDER_DESCRIPTOR_SET_H

#include <vulkan/vulkan.h>

#include <Core/DescriptorLayout.h>
#include <Core/DescriptorPool.h>
#include <Core/LogicalDevice.h>

namespace vrender::render
{
	class DescriptorSet
	{
	public:
		// Lifetime Control
		DescriptorSet(
			const vrender::render::LogicalDevice& logical_device,
			const vrender::render::DescriptorPool& descriptor_pool,
			const std::vector<vrender::render::DescriptorLayout>& descriptor_layouts
		);
		~DescriptorSet();

		DescriptorSet(const DescriptorSet&) = delete;
		DescriptorSet& operator=(const DescriptorSet&) = delete;

		DescriptorSet(DescriptorSet&& other) noexcept;
		DescriptorSet& operator=(DescriptorSet&& other) noexcept;

		// API Accessibility
		VkDescriptorSet get_descriptor_set() const;

	private:
		VkDescriptorSet descriptor_set;
	};
}

#endif