#include <EngineLayer/Core/SceneService.h>

// Lifetime Control
vrender::engine::SceneService::SceneService(
	vrender::engine::RenderService* render_service
)
{
	this->render_service = render_service;
}

// API Accessibility
void vrender::engine::SceneService::Start()
{

}
void vrender::engine::SceneService::Shutdown()
{

}
void vrender::engine::SceneService::Tick(float dt)
{
	std::vector<vrender::render::RenderObject> render_objects;
	render_objects.reserve(this->generator.live_count());

	this->generator.for_each_living_entry(
		[&](const vrender::engine::InstanceEntry& entry)
		{
			render_objects.emplace_back(vrender::render::RenderObject{
				.mesh = entry.mesh,
				.transform = ame::TRS(
					entry.transform.position,
					entry.transform.rotation,
					entry.transform.scale
				)
			});
		}
	);

	this->render_service->SubmitRenderObjects(render_objects);
}

void vrender::engine::SceneService::RegisterStaticMesh(
	std::string name,
	const std::vector<vrender::render::Vertex>& vertices,
	const std::vector<uint32_t>& indices
)
{
	vrender::render::MeshHandle handle = this->render_service->CreateMesh(vertices, indices);
	this->mesh_name_bindings.emplace(
		name,
		handle
	);
}
vrender::render::MeshHandle vrender::engine::SceneService::GetStaticMeshHandle(std::string name)
{
	try
	{
		return this->mesh_name_bindings.at(name);
	}
	catch (const std::exception& e)
	{
		std::cerr << "ERROR: SceneService Requested to Fetch Static Mesh by Name that Does Not Exist: " << name << std::endl;
		return vrender::utility::NULL_HANDLE;
	}
}
vrender::engine::InstanceHandle vrender::engine::SceneService::CreateInstanceOfMesh(std::string name)
{
	vrender::render::MeshHandle mesh_handle = this->GetStaticMeshHandle(name);
	if (mesh_handle == vrender::utility::NULL_HANDLE)
	{
		std::cerr << "ERROR: SceneService Requested to Create Instance of Non-Registered Mesh: " << name << std::endl;
		return vrender::utility::NULL_HANDLE;
	}

	vrender::engine::InstanceEntry entry{
		mesh_handle,
		vrender::engine::Transform{
			.scale{ 1.0f, 1.0f, 1.0f },
		}
	};
	vrender::engine::InstanceHandle instance_handle = this->generator.acquire_slot_handle(entry);
	return instance_handle;
}

vrender::engine::Transform& vrender::engine::SceneService::GetInstanceTransform(vrender::engine::InstanceHandle handle)
{
	vrender::utility::HandleComponents components = this->generator.decode_handle(handle);
	if (!(this->generator.handle_valid(handle) && this->generator.handle_alive(handle)))
	{
		vrender::engine::Transform transform{};
		if (!(this->generator.handle_valid(handle)))
		{
			std::cerr << "ERROR: SceneService Requested Transform of Invalid Instance Hande" << std::endl;
			return transform;
		}
		else
		{
			std::cerr << "ERROR: SceneService Requested Transform of Dead Instance Handle" << std::endl;
			return transform;
		}
	}

	return this->generator.entry_from_handle(handle).transform;
}