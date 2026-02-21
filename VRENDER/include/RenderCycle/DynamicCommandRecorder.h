#ifndef RENDER_DYNAMIC_COMMAND_RECORDER_H
#define RENDER_DYNAMIC_COMMAND_RECORDER_H

#include <vector>

#include <vulkan/vulkan.h>

#include <Core/GeometryArena.h>
#include <Core/ICommandRecorder.h>
#include <Core/Mesh.h>
#include <Core/Pipeline.h>

#include <RenderLayer/Configuration/FrameDescription.h>

namespace vrender::render
{
	class DynamicCommandRecorder : public vrender::render::ICommandRecorder
	{
	public:
		DynamicCommandRecorder(const vrender::render::Pipeline& pipeline, const vrender::render::GeometryArena& geometry_arena);

		void begin(VkCommandBuffer command_buffer, const vrender::render::config::FrameDescription& config) override;
		void record(
			const VkCommandBuffer command_buffer,
			const std::vector<VkDescriptorSet> descriptor_set,
			const std::vector<vrender::render::Mesh>& meshes
		) const override;
		void end(VkCommandBuffer command_buffer) override;

	private:
		const vrender::render::Pipeline* pipeline_ptr;
		const vrender::render::GeometryArena* geometry_arena_ptr;

		VkImageMemoryBarrier2 barrier{};
		VkDependencyInfo dependency_info{};
	};
}

#endif