#include <RenderLayer/Core/Memory/Suballocator.h>

// Lifetime Control
vrender::render::memory::Suballocator::Suballocator(
	vrender::render::memory::SuballocatorStrategy strategy,
	uint32_t start, 
	uint32_t size
)
	: strategy(strategy)
	, region_start(start)
	, region_size(size)
	, free_blocks({{ 0, size }})
{

}
vrender::render::memory::Suballocator::~Suballocator()
{

}

// API Accessibility
uint32_t vrender::render::memory::Suballocator::allocate(const uint32_t size)
{
	switch (this->strategy)
	{
	case vrender::render::memory::SuballocatorStrategy::BUMP:
		return this->alloc_bump(size);
	case vrender::render::memory::SuballocatorStrategy::FREE_LIST:
		return this->alloc_free_list(size);
	default:
		throw std::runtime_error("ERROR: VRENDER Encountered Unrecognized Suballocation Strategy");
	}
}
void vrender::render::memory::Suballocator::free(uint32_t offset, uint32_t size)
{
	if (this->strategy == vrender::render::memory::SuballocatorStrategy::BUMP)
	{
		// Bump data cannot be effectively cleared
		return;
	}

	// Find Correct Place for Block
	size_t insertion_pos = this->free_blocks.size();
	for (int i = 0; i < this->free_blocks.size(); i++)
	{
		if (offset < this->free_blocks[i].offset)
		{
			insertion_pos = i;
			break;
		}
	}

	// Insert and Check Merge Condition
	this->free_blocks.insert(
		this->free_blocks.begin() + insertion_pos,
		FreeBlock{ .offset = offset, .size = size }
	);
	FreeBlock& inserted = this->free_blocks[insertion_pos];

	if (insertion_pos != 0 && (
		this->free_blocks[insertion_pos - 1].offset +
		this->free_blocks[insertion_pos - 1].size ==
		this->free_blocks[insertion_pos].offset
	))
	{
		// Merge Left
		this->free_blocks[insertion_pos - 1].size += this->free_blocks[insertion_pos].size;
		this->free_blocks.erase(this->free_blocks.begin() + insertion_pos);
		
		// Prep for Right Check
		insertion_pos -= 1;
	}

	if (insertion_pos != this->free_blocks.size() - 1 && (
		this->free_blocks[insertion_pos].offset +
		this->free_blocks[insertion_pos].size ==
		this->free_blocks[insertion_pos + 1].offset
	))
	{
		// Merge Right
		this->free_blocks[insertion_pos].size += this->free_blocks[insertion_pos + 1].size;
		this->free_blocks.erase(this->free_blocks.begin() + insertion_pos + 1);
	}
}
void vrender::render::memory::Suballocator::reset()
{
	if (this->strategy == vrender::render::memory::SuballocatorStrategy::BUMP)
	{
		this->head = 0;
	}
	if (this->strategy == vrender::render::memory::SuballocatorStrategy::FREE_LIST)
	{
		this->free_blocks = std::vector<FreeBlock>{FreeBlock{ .offset = 0, .size = this->region_size }};
	}
}

// Allocation Strategies
uint32_t vrender::render::memory::Suballocator::alloc_bump(const uint32_t size)
{
	if (size > this->region_size - this->head)
	{
		std::cerr << "ERROR: VRENDER Cannot Allocate More Data Than Available in Suballocator" << std::endl;
		return UINT32_MAX;
	}

	uint32_t block_start = this->head;
	this->head += size;

	return this->region_start + block_start;
}
uint32_t vrender::render::memory::Suballocator::alloc_free_list(const uint32_t size)
{
	int best_block_index = -1;
	uint32_t minimal_fragmentation = UINT32_MAX;

	int index = 0;
	for (const vrender::render::memory::Suballocator::FreeBlock& block : this->free_blocks)
	{
		if (
			block.size >= size &&								// Ensure Block Contains Sufficient Space 
			block.size - size < minimal_fragmentation			// Check if Block is Better than Stored
		)
		{
			best_block_index = index;
			minimal_fragmentation = block.size - size;
		}

		index++;
	}

	if (best_block_index == -1)
	{
		std::cerr << "VRENDER: Unable to Allocate Requested Space" << std::endl;
		return UINT32_MAX;
	}

	uint32_t best_block_offset = this->free_blocks[best_block_index].offset;
	if (minimal_fragmentation == 0)
	{
		this->free_blocks.erase(this->free_blocks.begin() + best_block_index);
	}
	else
	{
		this->free_blocks[best_block_index].offset += size;
		this->free_blocks[best_block_index].size -= size;
	}

	return this->region_start + best_block_offset;
}

// Utility
void vrender::render::memory::Suballocator::defragment_blocks()
{
	// Sort + Merge Blocks as DE List, then convert to vector
	//		Start by Sorting by Offset
	std::sort(this->free_blocks.begin(), this->free_blocks.end(),
		[](const FreeBlock& a, const FreeBlock& b)
		{
			return a.offset < b.offset;
		}
	);

	//		Merge Touching Blocks
	std::vector<FreeBlock> merged;
	merged.reserve(free_blocks.size());

	for (const FreeBlock& block : this->free_blocks)
	{
		if (merged.empty())
		{
			merged.push_back(block);
			continue;
		}

		FreeBlock& end = merged.back();
		if (end.offset + end.size == block.offset)
		{
			end.size += block.size;
		}
		else
		{
			merged.push_back(block);
		}
	}

	free_blocks = std::move(merged);
}