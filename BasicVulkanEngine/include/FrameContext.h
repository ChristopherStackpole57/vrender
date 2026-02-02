#ifndef RENDER_FRAME_CONTEXT_H
#define RENDER_FRAME_CONTEXT_H

#include <Fence.h>
#include <LogicalDevice.h>
#include <Semaphore.h>

namespace vrender::render
{
	struct FrameContext
	{
	public:
		FrameContext(const vrender::render::LogicalDevice& logical_device)
			: in_flight(logical_device)
			, image_available(logical_device)
			, render_finished(logical_device) {};

		vrender::render::Fence in_flight;
		vrender::render::Semaphore image_available;
		vrender::render::Semaphore render_finished;
	};
}

#endif