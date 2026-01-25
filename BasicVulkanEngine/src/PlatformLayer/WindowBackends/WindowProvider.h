#ifndef PLATFORM_WBACK_WINDOW_PROVIDER_H
#define PLATFORM_WBACK_WINDOW_PROVIDER_H

#include <string>
#include <vector>
#include <utility>

#include <PlatformLayer/Utility/Event.h>
#include <PlatformLayer/Utility/ResizeState.h>
#include <PlatformLayer/Utility/WindowMode.h>

namespace vrender
{
	namespace platform
	{
		class WindowProvider
		{
		public:
			virtual ~WindowProvider() = default;
			
			// Event Processing
			virtual std::vector<vrender::platform::Event> poll_events() = 0;
			virtual std::vector<vrender::platform::Event> wait_events() = 0;
			virtual bool should_close() = 0;

			// Window State Queries
			virtual std::pair<int, int> get_window_size() const = 0;
			virtual std::pair<int, int> get_framebuffer_size() const = 0;
			virtual bool is_minimized() const = 0;
			virtual bool is_focused() const = 0;

			// Window State Mutation
			virtual void set_title(const std::string& title) = 0;
			virtual void set_window_mode(const vrender::platform::WindowMode mode) = 0;
			virtual void set_resizable(const bool resizable) = 0;
			virtual void set_visible(const bool visible) = 0;

			// Resize Signaling
			virtual bool was_resized() const = 0;
			virtual std::pair<int, int> get_last_resize_size() const = 0;

			// Native Handle Access
			// This will not be implemented for some time because (as much as possible) the platform layer should NOT be exposed to the render layer
		
			// Publicly Accessible Members
			std::vector<vrender::platform::Event> event_queue;
		protected:
			std::pair<int, int> windowed_size{ 0, 0 };
			std::pair<int, int> windowed_pos{ 0, 0 };
			vrender::platform::ResizeState resize_state{};
		};
	}
}

#endif