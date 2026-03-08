#ifndef RENDER_FRAME_DATA_H
#define RENDER_FRAME_DATA_H

#include <span>

#include <RenderLayer/Core/RenderObject.h>

namespace vrender::render
{
	struct FrameData
	{
		std::span<const vrender::render::RenderObject> objects;
	};
}

#endif