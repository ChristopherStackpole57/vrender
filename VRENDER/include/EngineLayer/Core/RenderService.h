#ifndef ENGINE_RENDER_SERVICE_H
#define ENGINE_RENDER_SERVICE_H

#include <vector>

#include <ame/AME.h>

#include <RenderLayer/Render.h>

#include <EngineLayer/Core/IService.h>

namespace vrender::engine
{
	class RenderService : public vrender::engine::IService
	{
	public:
		// Lifetime Control
		RenderService(
			vrender::platform::WindowProvider& window_provider_ptr,
			vrender::platform::WindowSurfaceProvider& surface_provider_ptr,
			vrender::render::config::InstanceConfig& instance_config
		);

		// API Accessibility
		void Start() override;
		void Shutdown() override;
		void Tick(float dt) override;

		vrender::render::MeshToken CreateMesh(
			std::vector<vrender::render::Vertex> vertices,
			std::vector<uint32_t> indices
		);

		void SubmitRenderObject(vrender::render::RenderObject render_object);
		void SubmitRenderObjects(std::vector<vrender::render::RenderObject>& render_objects);
	private:
		vrender::render::Renderer renderer;

		std::vector<vrender::render::RenderObject> render_objects;
	};
}

#endif