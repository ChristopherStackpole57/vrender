#ifndef RENDER_BINDLESS_REGISTRY_H
#define RENDER_BINDLESS_REGISTRY_H

#include <unordered_map>

#include <vulkan/vulkan.h>

#include <RenderLayer/Core/DescriptorLayout.h>
#include <RenderLayer/Core/DescriptorPool.h>
#include <RenderLayer/Core/DescriptorSet.h>

#include <RenderLayer/Core/Memory/Buffer.h>
#include <RenderLayer/Core/Memory/Suballocator.h>

#include <Utility/Generator.h>

namespace vrender::render
{
	struct BREntry
	{
		const vrender::render::memory::Buffer* buffer;
		VkDeviceSize offset;
		VkDeviceSize range;

		uint32_t binding;
		uint32_t descriptor_index;
	};
	typedef vrender::utility::Handle BRHandle;

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

		BindlessRegistry(BindlessRegistry&& other) noexcept;
		BindlessRegistry& operator=(BindlessRegistry&& other) noexcept;

		// API Accessibility
		BRHandle register_uniform_buffer(const vrender::render::memory::Buffer& buffer, uint32_t binding);
		BRHandle register_storage_buffer(const vrender::render::memory::Buffer& buffer, uint32_t binding);
		void update_uniform_buffer(vrender::render::BRHandle handle);
		void update_storage_buffer(vrender::render::BRHandle handle);
		// free

		VkDescriptorSet get_descriptor_set() const;
	private:
		vrender::render::DescriptorSet descriptor_set;
		std::unordered_map<uint32_t, vrender::render::memory::Suballocator> uniform_buffer_suballocators;
		std::unordered_map<uint32_t, vrender::render::memory::Suballocator> storage_buffer_suballocators;

		const vrender::render::LogicalDevice* logical_device_ptr;

		vrender::utility::Generator<BREntry> generator;
	};
}

#endif