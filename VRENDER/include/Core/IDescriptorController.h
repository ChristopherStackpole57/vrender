#ifndef RENDER_INTERFACE_DESCRIPTOR_CONTROLLER_H
#define RENDER_INTERFACE_DESCRIPTOR_CONTROLLER_H

#include <vulkan/vulkan.h>

namespace vrender::render
{
	struct DescriptorPoolRequirements
	{
		uint32_t max_sets;
		std::vector<VkDescriptorPoolSize> pool_sizes;
	};

	struct FrameDescriptorSets
	{
		std::vector<VkDescriptorSet> global_descriptors;
		std::vector<VkDescriptorSet> frame_descriptors;
	};
	struct FrameDescriptorInputs
	{
		VkBuffer global_ubo;
		VkBuffer frame_ubo;
	};

	class IDescriptorController
	{
	public:
		virtual DescriptorPoolRequirements get_pool_sizes() const = 0;
		virtual FrameDescriptorSets prepare_descriptors(
			VkDescriptorPool descriptor_pool,
			FrameDescriptorInputs inputs
		) const = 0;
	private:
	};
}

#endif