#ifdef VRENDER_HAS_SDL

#include "SDLWindowBackend.h"

// Lifetime Control
vrender::platform::SDLWindowBackend::SDLWindowBackend()
{

}
vrender::platform::SDLWindowBackend::~SDLWindowBackend()
{

}

// WindowProvider Contract


// WindowServiceProvider Contract
std::vector<std::string> vrender::platform::SDLWindowBackend::get_required_instance_extensions() const
{
	// TODO: Implement SDL Backend
	return std::vector<std::string>{};
}
VkSurfaceKHR vrender::platform::SDLWindowBackend::create_surface(VkInstance instance) const
{
	// TODO: Implement SDL Backend
	return VK_NULL_HANDLE;
}
bool vrender::platform::SDLWindowBackend::destroy_surface(VkInstance instance, VkSurfaceKHR surface) const
{
	// TODO: Implement SDL Backend
	return false;
}

#endif