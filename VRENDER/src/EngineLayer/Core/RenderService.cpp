#include <EngineLayer/Core/RenderService.h>

// Lifetime Control
vrender::engine::RenderService::RenderService(
	vrender::platform::WindowProvider& window_provider_ptr,
	vrender::platform::WindowSurfaceProvider& surface_provider_ptr,
	vrender::render::config::InstanceConfig& instance_config
)
	: renderer(
		window_provider_ptr,
		surface_provider_ptr,
		instance_config
	)
{

}

// API Accessibility
void vrender::engine::RenderService::Start()
{

}
void vrender::engine::RenderService::Shutdown()
{

}
void vrender::engine::RenderService::Tick(float dt)
{
	this->renderer.step(
		vrender::render::FrameData{ this->render_objects }
	);

	this->render_objects.clear();
}

vrender::render::MeshToken vrender::engine::RenderService::CreateMesh(
	std::vector<vrender::render::Vertex> vertices,
	std::vector<uint32_t> indices
)
{
	vrender::render::MeshToken mesh = this->renderer.get_geometry_arena().create_static_mesh(
		vertices,
		indices
	);

	return mesh;
}

void vrender::engine::RenderService::SubmitRenderObject(vrender::render::RenderObject render_object)
{
	this->render_objects.emplace_back(render_object);
}
void vrender::engine::RenderService::SubmitRenderObjects(std::vector<vrender::render::RenderObject>& objects)
{
	this->render_objects.insert(
		this->render_objects.begin(),
		objects.begin(),
		objects.end()
	);
}