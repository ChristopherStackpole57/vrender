#ifdef VRENDER_HAS_WIN32

#include "WIN32WindowBackend.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vulkan/vulkan_win32.h>

// Lifetime Control
vrender::platform::WIN32WindowBackend::WIN32WindowBackend()
{

}
vrender::platform::WIN32WindowBackend::~WIN32WindowBackend()
{

}

// WindowProvider Contract

// WindowSurfaceProvider Contract
std::vector<std::string> vrender::platform::WIN32WindowBackend::get_required_instance_extensions() const
{
	return std::vector<std::string>{};
}
VkSurfaceKHR vrender::platform::WIN32WindowBackend::create_surface(VkInstance intance) const
{

	return NULL;
}
bool vrender::platform::WIN32WindowBackend::destroy_surface(VkInstance instance, VkSurfaceKHR surface) const
{
	return false;
}

#endif