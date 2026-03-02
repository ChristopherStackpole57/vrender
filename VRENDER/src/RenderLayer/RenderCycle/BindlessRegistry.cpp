#include <RenderLayer/RenderCycle/BindlessRegistry.h>

// Lifetime Control
vrender::render::BindlessRegistry::BindlessRegistry(
	const vrender::render::LogicalDevice& logical_device,
	const vrender::render::DescriptorPool& descriptor_pool,
	const std::vector<vrender::render::DescriptorLayout>& descriptor_layouts
)
	: descriptor_set(logical_device, descriptor_pool, descriptor_layouts)
	, logical_device_ptr(&logical_device)
{
	for (const vrender::render::DescriptorLayout& layout : descriptor_layouts)
	{
		const std::vector<VkDescriptorSetLayoutBinding> bindings = layout.get_bindings();
		for (const VkDescriptorSetLayoutBinding& binding : bindings)
		{
			if (binding.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
			{
				this->uniform_buffer_suballocators.emplace(
					binding.binding,
					vrender::render::memory::Suballocator(
						vrender::render::memory::SuballocatorStrategy::FREE_LIST,
						0,
						binding.descriptorCount
					)
				);
			} else if (binding.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
			{
				this->storage_buffer_suballocators.emplace(
					binding.binding,
					vrender::render::memory::Suballocator(
						vrender::render::memory::SuballocatorStrategy::FREE_LIST,
						0,
						binding.descriptorCount
					)
				);
			}
		}
	}
}
vrender::render::BindlessRegistry::~BindlessRegistry()
{

}

vrender::render::BindlessRegistry::BindlessRegistry(vrender::render::BindlessRegistry&& other) noexcept
	: descriptor_set(std::move(other.descriptor_set))
	, storage_buffer_suballocators(std::move(other.storage_buffer_suballocators))
	, logical_device_ptr(other.logical_device_ptr)
	, slots(other.slots)
	, free_indices(other.free_indices)
{
	//other.storage_buffer_suballocators = {};
}
vrender::render::BindlessRegistry& vrender::render::BindlessRegistry::operator=(vrender::render::BindlessRegistry&& other) noexcept
{
	if (this != &other)
	{
		this->descriptor_set = std::move(other.descriptor_set);
		this->storage_buffer_suballocators = std::move(other.storage_buffer_suballocators);
		this->logical_device_ptr = other.logical_device_ptr;
		this->slots = other.slots;
		this->free_indices = other.free_indices;

		//other.storage_buffer_suballocators = {};
	}

	return *this;
}

// API Accessibility
vrender::render::BRToken vrender::render::BindlessRegistry::register_uniform_buffer(
	const vrender::render::memory::Buffer& buffer,
	uint32_t binding
)
{
	uint32_t index = this->uniform_buffer_suballocators.at(binding).allocate(1);
	if (index == UINT32_MAX)
	{
		throw std::runtime_error("ERROR: Bindless Buffer Capacity Exhausted");
	}

	VkDescriptorBufferInfo buffer_info{};
	buffer_info.buffer = buffer.get_buffer();
	buffer_info.offset = 0;
	buffer_info.range = buffer.get_size();

	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.dstSet = this->descriptor_set.get_descriptor_set();
	write.dstBinding = binding;
	write.dstArrayElement = index;
	write.descriptorCount = 1;
	write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	write.pBufferInfo = &buffer_info;

	vkUpdateDescriptorSets(
		this->logical_device_ptr->get_logical_device(),
		1,
		&write,
		0,
		nullptr
	);

	return this->acquire_slot_token(vrender::render::BREntry{
		.buffer = &buffer,
		.offset = 0,
		.range = buffer.get_size(),
		.binding = binding,
		.descriptor_index = index
	});
}
vrender::render::BRToken vrender::render::BindlessRegistry::register_storage_buffer(
	const vrender::render::memory::Buffer& buffer,
	uint32_t binding
)
{
	uint32_t index = this->storage_buffer_suballocators.at(binding).allocate(1);
	if (index == UINT32_MAX)
	{
		throw std::runtime_error("ERROR: Bindless Buffer Capacity Exhausted");
	}

	VkDescriptorBufferInfo buffer_info{};
	buffer_info.buffer = buffer.get_buffer();
	buffer_info.offset = 0;
	buffer_info.range = buffer.get_size();

	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.dstSet = this->descriptor_set.get_descriptor_set();
	write.dstBinding = binding;
	write.dstArrayElement = index;
	write.descriptorCount = 1;
	write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	write.pBufferInfo = &buffer_info;

	vkUpdateDescriptorSets(
		this->logical_device_ptr->get_logical_device(),
		1,
		&write,
		0,
		nullptr
	);

	return this->acquire_slot_token(vrender::render::BREntry{
		.buffer = &buffer,
		.offset = 0,
		.range = buffer.get_size(),
		.binding = binding,
		.descriptor_index = index
	});
}
void vrender::render::BindlessRegistry::update_uniform_buffer(vrender::render::BRToken token)
{
	// Validate Token
	if (
		!(this->token_valid(token)) ||
		!(this->token_alive(token))
		)
	{
		std::cerr << "ERROR: Bindless Registry Requested to Write Out Of Generation Buffer" << std::endl;
		return;
	}

	// Write Buffer
	vrender::render::BREntry& entry = this->slot_from_token(token).entry;
	const vrender::render::memory::Buffer& buffer = *entry.buffer;

	VkDescriptorBufferInfo buffer_info{};
	buffer_info.buffer = buffer.get_buffer();
	buffer_info.offset = 0;
	buffer_info.range = buffer.get_size();

	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.dstSet = this->descriptor_set.get_descriptor_set();
	write.dstBinding = entry.binding;
	write.dstArrayElement = entry.descriptor_index;
	write.descriptorCount = 1;
	write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	write.pBufferInfo = &buffer_info;

	vkUpdateDescriptorSets(
		this->logical_device_ptr->get_logical_device(),
		1,
		&write,
		0,
		nullptr
	);
}
void vrender::render::BindlessRegistry::update_storage_buffer(vrender::render::BRToken token)
{
	// Validate Token
	if (
		!(this->token_valid(token)) ||
		!(this->token_alive(token))
	)
	{
		std::cerr << "ERROR: Bindless Registry Requested to Write Out Of Generation Buffer" << std::endl;
		return;
	}

	// Write Buffer
	vrender::render::BREntry& entry = this->slot_from_token(token).entry;
	const vrender::render::memory::Buffer& buffer = *entry.buffer;

	VkDescriptorBufferInfo buffer_info{};
	buffer_info.buffer = buffer.get_buffer();
	buffer_info.offset = 0;
	buffer_info.range = buffer.get_size();

	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.dstSet = this->descriptor_set.get_descriptor_set();
	write.dstBinding = entry.binding;
	write.dstArrayElement = entry.descriptor_index;
	write.descriptorCount = 1;
	write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	write.pBufferInfo = &buffer_info;

	vkUpdateDescriptorSets(
		this->logical_device_ptr->get_logical_device(),
		1,
		&write,
		0,
		nullptr
	);
}

VkDescriptorSet vrender::render::BindlessRegistry::get_descriptor_set() const
{
	return this->descriptor_set.get_descriptor_set();
}

// Token Arena
vrender::render::BRToken vrender::render::BindlessRegistry::encode_token(uint64_t index, uint64_t generation)
{
	return (index << 32) | generation;
}
vrender::render::BRTokenComponents vrender::render::BindlessRegistry::decode_token(vrender::render::BRToken token)
{
	return {
		static_cast<uint32_t>(token >> 32),
		static_cast<uint32_t>(token)
	};
}
vrender::render::BRToken vrender::render::BindlessRegistry::acquire_slot_token(vrender::render::BREntry	entry)
{
	uint32_t index;
	if (this->free_indices.size() == 0)
	{
		// Must Generate a New Slot
		this->slots.push_back({
			entry,
			0,
			true
		});

		index = static_cast<uint32_t>(this->slots.size() - 1);
	}
	else
	{
		index = this->free_indices.back();
		this->free_indices.pop_back();

		this->slots[index].entry = entry;
		this->slots[index].generation++;
		this->slots[index].alive = true;
	}

	uint32_t generation = this->slots[index].generation;

	return encode_token(index, generation);
}
vrender::render::BRSlot& vrender::render::BindlessRegistry::slot_from_token(vrender::render::BRToken token)
{
	vrender::render::BRTokenComponents comps = decode_token(token);
	return this->slots[comps.index];
}

bool vrender::render::BindlessRegistry::token_valid(vrender::render::BRToken token)
{
	vrender::render::BRTokenComponents comps = this->decode_token(token);
	return comps.index < this->slots.size();
}
bool vrender::render::BindlessRegistry::token_alive(vrender::render::BRToken token)
{
	vrender::render::BRTokenComponents comps = this->decode_token(token);
	vrender::render::BRSlot& slot = this->slots[comps.index];

	return
		slot.alive &&
		slot.generation == comps.generation;
}