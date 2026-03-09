#ifndef RENDER_CPU_ACCESS_H
#define RENDER_CPU_ACCESS_H

namespace vrender::render::memory
{
	enum class CPUAccess
	{
		NONE,
		WRITE_ONCE,
		WRITE_OFTEN
	};
}

#endif