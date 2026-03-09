#include <RenderLayer/Core/Memory/Image.h>

// Lifetime Control
vrender::render::memory::Image::Image(
	vrender::render::memory::Allocator& allocator,
	vrender::render::memory::ImageDesc& desc
)
	: desc(desc)
	, allocator_ptr(&allocator)
{
	vrender::render::memory::ImageAllocationResult alloc_result = allocator.allocate_image(desc);
	this->image = alloc_result.image;
	this->handle = alloc_result.handle;
}
vrender::render::memory::Image::~Image()
{
	if (this->handle == vrender::utility::NULL_HANDLE)
	{
		return;
	}

	this->allocator_ptr->free_image(this->handle);

	this->image = VK_NULL_HANDLE;
	this->handle = vrender::utility::NULL_HANDLE;
}

vrender::render::memory::Image::Image(vrender::render::memory::Image&& other) noexcept
	: desc(other.desc)
	, image(other.image)
	, handle(other.handle)
	, allocator_ptr(other.allocator_ptr)
{
	other.image = VK_NULL_HANDLE;
	other.handle = vrender::utility::NULL_HANDLE;
}
vrender::render::memory::Image& vrender::render::memory::Image::operator=(vrender::render::memory::Image&& other) noexcept
{
	if (this != &other)
	{
		this->desc = other.desc;
		this->image = other.image;
		this->handle = other.handle;
		this->allocator_ptr = other.allocator_ptr;

		other.image = VK_NULL_HANDLE;
		other.handle = vrender::utility::NULL_HANDLE;
	}

	return *this;
}

// API Accessibility
VkImage vrender::render::memory::Image::get_image() const
{
	return this->image;
}
//VkDeviceSize vrender::render::memory::Image::get_size() const
//{
	//return 
//}

void vrender::render::memory::Image::write()
{

}