#ifndef ENGINE_RENDER_SERVICE_H
#define ENGINE_RENDER_SERVICE_H

#include <EngineLayer/Core/IService.h>

namespace vrender::engine
{
	class RenderService : public vrender::engine::IService
	{
	public:
		// Lifetime Control
		RenderService();

		// API Accessibility
		void Start() override;
		void Shutdown() override;
		void Tick(float dt) override;

	private:
	};
}

#endif