#ifndef RENDER_DYNAMIC_COMMAND_RECORDER_H
#define RENDER_DYNAMIC_COMMAND_RECORDER_H

#include <vector>

#include <vulkan/vulkan.h>

#include <Core/ICommandRecorder.h>
#include <Core/IFrameTarget.h>
#include <Core/Mesh.h>

namespace vrender::render
{
	class DynamicCommandRecorder : public vrender::render::ICommandRecorder
	{
		void record(
			const VkCommandBuffer command_buffer,
			const vrender::render::IFrameTarget& frame_target,
			const std::vector<VkDescriptorSet> descriptor_sets,
			const std::vector<vrender::render::Mesh>& meshes
		) override;
	};
}

#endif