#ifndef RENDER_RENDER_PASS_COMMAND_RECORDER_H
#define RENDER_RENDER_PASS_COMMAND_RECORDER_H

#include <vulkan/vulkan.h>

#include <Core/GeometryArena.h>
#include <Core/ICommandRecorder.h>
#include <Core/LogicalDevice.h>
#include <Core/Mesh.h>
#include <Core/PhysicalDevice.h>
#include <Core/Memory/Buffer.h>

#include <LegacyRender/Pipeline.h>

namespace vrender::render
{
	class RenderPassCommandRecorder : public vrender::render::ICommandRecorder
	{
	public:
		// Lifetime Control
		RenderPassCommandRecorder(
			const vrender::render::LogicalDevice& logical_device,
			const vrender::render::PhysicalDevice& physical_device,
			const vrender::render::Pipeline& pipeline,
			const vrender::render::GeometryArena& geometry_arena
		);
		~RenderPassCommandRecorder();

		// API Accessibility
		void record(
			const VkCommandBuffer command_buffer,
			const vrender::render::IFrameTarget& frame_target,
			const std::vector<VkDescriptorSet> descriptor_sets,
			const std::vector<vrender::render::Mesh>& meshes
		) const override;
	private:
		const vrender::render::Pipeline& pipeline;
		const vrender::render::LogicalDevice* logical_device_ptr;
		const vrender::render::PhysicalDevice* physical_device_ptr;
		const vrender::render::GeometryArena* geometry_arena_ptr;
	};
}

#endif