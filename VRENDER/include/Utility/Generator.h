#ifndef UTILITY_GENERATOR_H
#define UTILITY_GENERATOR_H

#include <cstdint>
#include <vector>

namespace vrender::utility
{
	template <typename EntryType>
	struct Slot
	{
		EntryType entry;
		uint32_t generation;
		bool alive;
	};
	typedef uint64_t Handle;
	struct HandleComponents
	{
		uint32_t generation;
		uint32_t index;
	};

	constexpr uint64_t NULL_HANDLE = UINT64_MAX;
	constexpr uint32_t NULL_HANDLE_INDEX = UINT32_MAX;
	constexpr uint32_t NULL_HANDLE_GENERATION = UINT32_MAX;

	template <typename EntryType>
	class Generator
	{
	public:
		vrender::utility::Handle encode_handle(uint64_t index, uint64_t generation) const;
		vrender::utility::HandleComponents decode_handle(vrender::utility::Handle handle) const;

		vrender::utility::Handle acquire_slot_handle(EntryType entry);
		EntryType& entry_from_handle(vrender::utility::Handle handle);

		template <typename Func>
		void for_each_living_entry(Func&& func);
		
		uint32_t live_count() const;

		bool handle_valid(vrender::utility::Handle handle) const;
		bool handle_alive(vrender::utility::Handle handle) const;
	private:
		std::vector<vrender::utility::Slot<EntryType>> slots;
		std::vector<uint32_t> free_indices;
	};
}

template <typename EntryType>
vrender::utility::Handle vrender::utility::Generator<EntryType>::encode_handle(uint64_t index, uint64_t generation) const
{
	return (generation << 32) | index;
}

template <typename EntryType>
vrender::utility::HandleComponents vrender::utility::Generator<EntryType>::decode_handle(vrender::utility::Handle handle) const
{
	return {
		.generation = static_cast<uint32_t>(handle >> 32),
		.index = static_cast<uint32_t>(handle)
	};
}

template <typename EntryType>
vrender::utility::Handle vrender::utility::Generator<EntryType>::acquire_slot_handle(EntryType entry)
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

	return encode_handle(index, generation);
}

template <typename EntryType>
EntryType& vrender::utility::Generator<EntryType>::entry_from_handle(vrender::utility::Handle handle)
{
	vrender::utility::HandleComponents comps = decode_handle(handle);
	return this->slots[comps.index].entry;
}

template <typename EntryType>
template <typename Func>
void vrender::utility::Generator<EntryType>::for_each_living_entry(Func&& func)
{
	for (vrender::utility::Slot<EntryType>& slot : slots)
	{
		if (slot.alive)
		{
			func(slot.entry);
		}
	}
}

template <typename EntryType>
uint32_t vrender::utility::Generator<EntryType>::live_count() const
{
	return this->slots.size() - this->free_indices.size();
}

template <typename EntryType>
bool vrender::utility::Generator<EntryType>::handle_valid(vrender::utility::Handle handle) const
{
	vrender::utility::HandleComponents comps = this->decode_handle(handle);
	return comps.index < this->slots.size();
}

template <typename EntryType>
bool vrender::utility::Generator<EntryType>::handle_alive(vrender::utility::Handle handle) const
{
	vrender::utility::HandleComponents comps = this->decode_handle(handle);
	const vrender::utility::Slot<EntryType>& slot = this->slots[comps.index];

	return
		slot.alive &&
		slot.generation == comps.generation;
}

#endif