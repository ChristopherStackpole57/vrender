#ifdef VRENDER_HAS_GLFW

	#ifndef PLATFORM_WBACK_GLFW
	#define PLATFORM_WBACK_GLFW

	#include "WindowProvider.h"
	#include "WindowSurfaceProvider.h"

	#define GLFW_INCLUDE_VULKAN
	#include <GLFW/glfw3.h>

	namespace vrender
	{
		namespace platform
		{
			class GLFWWindowBackend final
				: public vrender::platform::WindowProvider
				, public vrender::platform::WindowSurfaceProvider
			{
			public:
				// Lifetime Control
				GLFWWindowBackend();
				~GLFWWindowBackend();

				// WinowProvider Contract
				//		Event Processing
				std::vector<vrender::platform::Event> poll_events() override;
				std::vector<vrender::platform::Event> wait_events() override;
				bool should_close() override;

				//		Window State Queries
				std::pair<int, int> get_window_size() const override;
				std::pair<int, int> get_framebuffer_size() const override;
				bool is_minimized() const override;
				bool is_focused() const override;

				//		Window State Mutation
				void set_title(const std::string& title) override;
				void set_window_mode(const vrender::platform::WindowMode mode) override;
				void set_resizable(const bool resizabe) override;
				void set_visible(const bool visible) override;

				//		Resize Signaling
				bool was_resized() const override;
				std::pair<int, int> get_last_resize_size() const override;

				// WindowSurfaceProvider Contract
				std::vector<std::string> get_required_instance_extensions() const override;
				VkSurfaceKHR create_surface(VkInstance instance) const override;
				bool destroy_surface(VkInstance instance, VkSurfaceKHR surface) const override;
			private:
				GLFWwindow* glfw_window = nullptr;
			};
		};
	};

	#endif

#endif