#ifndef RENDER_INTERFACE_COMMAND_RECORDER_H
#define RENDER_INTERFACE_COMMAND_RECORDER_H

#include <vector>

#include <vulkan/vulkan.h>

#include <RenderLayer/Core/Mesh.h>

#include <RenderLayer/Configuration/FrameDescription.h>

namespace vrender::render
{
	class ICommandRecorder
	{
	public:
		virtual ~ICommandRecorder() = default;

		virtual void begin(VkCommandBuffer command_buffer, const vrender::render::config::FrameDescription& config) = 0;
		virtual void record(
			const VkCommandBuffer command_buffer, 
			const std::vector<VkDescriptorSet> descriptor_set,
			const std::vector<vrender::render::Mesh>& meshes
		) const = 0;
		virtual void end(VkCommandBuffer command_buffer) = 0;
	};
}

#endif