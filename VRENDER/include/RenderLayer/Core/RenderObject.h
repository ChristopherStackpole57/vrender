#ifndef RENDER_RENDER_OBJECT_H
#define RENDER_RENDER_OBJECT_H

#include <ame/AME.h>

#include <RenderLayer/Core/Mesh.h>

namespace vrender::render
{
	struct RenderObject
	{
		const vrender::render::Mesh& mesh;
		const ame::mat4f transform;
	};
}

#endif