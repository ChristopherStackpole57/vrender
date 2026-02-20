#ifndef RENDER_BINDLESS_REGISTRY_H
#define RENDER_BINDLESS_REGISTRY_H

#include <unordered_map>

#include <vulkan/vulkan.h>

#include <Core/DescriptorLayout.h>
#include <Core/DescriptorPool.h>
#include <Core/DescriptorSet.h>

#include <Core/Memory/Buffer.h>
#include <Core/Memory/Suballocator.h>

namespace vrender::render
{
	class BindlessRegistry
	{
	public:
		// Lifetime Control
		BindlessRegistry(
			const vrender::render::LogicalDevice& logical_device,
			const vrender::render::DescriptorPool& descriptor_pool,
			const std::vector<vrender::render::DescriptorLayout>& descriptor_layouts
		);
		~BindlessRegistry();

		BindlessRegistry(const BindlessRegistry&) = delete;
		BindlessRegistry& operator=(const BindlessRegistry&) = delete;

		//BindlessRegistry(BindlessRegistry&& other) noexecpt;
		//BindlessRegistry& operator=(BindlessRegistry&& other) noexcept;

		// API Accessibility
		uint32_t register_storage_buffer(const vrender::render::memory::Buffer& buffer, uint32_t binding);
		void update_storage_buffer(uint32_t index);
	private:
		vrender::render::DescriptorSet descriptor_set;
		std::unordered_map<uint32_t, vrender::render::memory::Suballocator> storage_buffer_suballocators;

		const vrender::render::LogicalDevice* logical_device_ptr;
	};
}

#endif