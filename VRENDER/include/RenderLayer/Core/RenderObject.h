#ifndef RENDER_RENDER_OBJECT_H
#define RENDER_RENDER_OBJECT_H

#include <ame/AME.h>

#include <RenderLayer/Core/Mesh.h>

namespace vrender::render
{
	struct RenderObject
	{
		vrender::render::MeshToken mesh;
		ame::mat4f transform;
	};
}

#endif