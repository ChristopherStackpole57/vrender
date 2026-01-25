#ifndef PLATFORM_RESIZE_STATE_H
#define PLATFORM_RESIZE_STATE_H

namespace vrender
{
	namespace platform
	{
		struct ResizeState
		{
			std::pair<int, int> size{ 0, 0 };
			std::pair<int, int> previous_size{ 0, 0 };
			bool resized = false;
		};
	}
}

#endif