#ifndef PLATFORM_EVENT_H
#define PLATFORM_EVENT_H

// Universal Event Wrapper
// The goal here is to unify the event structure of many different possible backends
// There may not be a 100% translation between each backend's event structure and this one, but it will encapsulate all of the major
// information needed for use in the the renderer

namespace vrender
{
	namespace platform
	{
		enum class EventType
		{
			None,

			WindowClose,
			WindowResize,
			WindowFocus,

			KeyPress,
			KeyRelease,
			MouseMove,
			MouseButtonPress,
			MouseButtonRelease,
			MouseWheel
		};

		struct Event
		{
			EventType type;

			// A union is being utilized to simplify the struct construction of the event wrapper
			union
			{
				struct { int width, height; } resize;

				struct { int key_code; } key;

				struct { int x, y; } mouse_move;
				struct { int button, x, y; } mouse_button;
				struct { float delta_x, delta_y; } mouse_scroll;
			};
		};
	}
}

#endif