#ifndef RENDER_RENDER_PASS_DESCRIPTOR_CONTROLLER_H
#define RENDER_RENDER_PASS_DESCRIPTOR_CONTROLLER_H

#include <unordered_map>
#include <vector>

#include <vulkan/vulkan.h>

#include <Core/IDescriptorController.h>
#include <Core/LogicalDevice.h>

#include <LegacyRender/DescriptorLayout.h>

namespace vrender::render
{
	class RenderPassDescriptorController : public vrender::render::IDescriptorController
	{
	public:
		// Lifetime Control
		RenderPassDescriptorController(
			const vrender::render::LogicalDevice& logical_device,
			// globals
			const std::vector<vrender::render::DescriptorLayout>& frame_layouts
		);

		// API Accessiblity
		vrender::render::DescriptorPoolRequirements get_pool_sizes() const override;
		vrender::render::FrameDescriptorSets prepare_descriptors(
			VkDescriptorPool descriptor_pool,
			vrender::render::FrameDescriptorInputs inputs
		) const override;
	private:
		const uint16_t MAX_OBJECTS_PER_FRAME = 500;

		std::vector<VkDescriptorPoolSize> pool_sizes;
		std::vector<VkDescriptorSetLayout> global_layouts;
		std::vector<VkDescriptorSetLayout> frame_layouts;
	
		const vrender::render::LogicalDevice* logical_device_ptr;
	};
}

#endif