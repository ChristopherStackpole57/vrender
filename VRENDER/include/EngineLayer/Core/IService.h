#ifndef ENGINE_INTERFACE_SERVICE_H
#define ENGINE_INTERFACE_SERVICE_H

#include <string>

namespace vrender::engine
{
	class IService
	{
	public:
		virtual ~IService() = default;

		virtual void Start() = 0;
		virtual void Shutdown() = 0;
		virtual void Tick(float dt) {};
	};
}

#endif