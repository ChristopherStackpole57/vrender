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
	struct BREntry
	{
		const vrender::render::memory::Buffer* buffer;
		VkDeviceSize offset;
		VkDeviceSize range;

		uint32_t binding;
		uint32_t descriptor_index;
	};
	struct BRSlot
	{
		BREntry entry;
		uint32_t generation;
		bool alive;
	};
	typedef uint64_t BRToken;
	struct BRTokenComponents
	{
		uint32_t generation;
		uint32_t index;
	};

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
		BRToken register_storage_buffer(const vrender::render::memory::Buffer& buffer, uint32_t binding);
		void update_storage_buffer(vrender::render::BRToken token);
		// free

		VkDescriptorSet get_descriptor_set() const;
	private:
		BRToken encode_token(uint64_t index, uint64_t generation);
		BRTokenComponents decode_token(BRToken token);
		BRToken acquire_slot_token(BREntry entry);
		BRSlot& slot_from_token(BRToken token);

		bool token_valid(BRToken token);
		bool token_alive(BRToken token);

		vrender::render::DescriptorSet descriptor_set;
		std::unordered_map<uint32_t, vrender::render::memory::Suballocator> storage_buffer_suballocators;

		const vrender::render::LogicalDevice* logical_device_ptr;

		std::vector<BRSlot> slots;
		std::vector<uint32_t> free_indices;
	};
}

#endif