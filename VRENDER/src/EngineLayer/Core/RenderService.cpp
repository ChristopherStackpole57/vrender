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
	std::vector<vrender::render::RenderObject> objects;
	objects.reserve(this->meshes.size());
	for (const vrender::render::Mesh& mesh : this->meshes)
	{
		objects.emplace_back(
			mesh,
			ame::TRS(
				ame::vec3f{  1.0f, - 1.0f, - 4.0f },
				ame::vec3f{ 40.0f,  25.0f,  15.0f },
				ame::vec3f{  1.0f,   1.0f,   1.0f }
			)
		);
	}

	this->renderer.step(
		vrender::render::FrameData{ objects }
	);
}

void vrender::engine::RenderService::CreateMesh(
	std::vector<vrender::render::Vertex> vertices,
	std::vector<uint32_t> indices
)
{
	vrender::render::Mesh mesh = this->renderer.get_geometry_arena().create_static_mesh(
		vertices,
		indices
	);

	this->meshes.emplace_back(mesh);
}