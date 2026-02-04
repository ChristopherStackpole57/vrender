#ifndef RENDER_DESCRIPTOR_LAYOUT_H
#define RENDER_DESCRIPTOR_LAYOUT_H

#include <vector>

#include <vulkan/vulkan.h>

#include <Core/LogicalDevice.h>

#include <RenderLayer/Configuration/BindingConfiguration.h>

namespace vrender::render
{
	class DescriptorLayout
	{
	public:
		// Lifetime Control
		DescriptorLayout(
			const vrender::render::LogicalDevice& logical_device,
			const std::vector<vrender::render::config::BindingConfiguration>& bindings
		);
		~DescriptorLayout();

		DescriptorLayout(const DescriptorLayout&) = delete;
		DescriptorLayout& operator=(const DescriptorLayout&) = delete;

		DescriptorLayout(DescriptorLayout&& other) noexcept;
		DescriptorLayout& operator=(DescriptorLayout&& other) noexcept;

		// API Accessibility
		VkDescriptorSetLayout get_descriptor_layout() const;
	private:
		VkDescriptorSetLayout descriptor_layout;
		const vrender::render::LogicalDevice* logical_device_ptr;
	};
}

#endif