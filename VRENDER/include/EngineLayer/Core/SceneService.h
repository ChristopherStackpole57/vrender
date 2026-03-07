#ifndef ENGINE_SCENE_SERVICE_H
#define ENGINE_SCENE_SERVICE_H

#include <unordered_map>
#include <vector>

#include <ame/AME.h>

#include <RenderLayer/Core/Mesh.h>
#include <RenderLayer/Core/Vertex.h>

#include <EngineLayer/Core/IService.h>
#include <EngineLayer/Core/RenderService.h>

#include <Utility/Generator.h>

namespace vrender::engine
{
	struct Transform
	{
		ame::vec3f position;
		ame::vec3f rotation;
		ame::vec3f scale;
	};

	struct InstanceEntry
	{
		vrender::render::MeshHandle mesh;
		vrender::engine::Transform transform;
	};
	typedef vrender::utility::Handle InstanceHandle;
	

	class SceneService : public vrender::engine::IService
	{
	public:
		// Lifetime Control
		SceneService(vrender::engine::RenderService* render_service);

		// API Accessiblity
		void Start() override;
		void Shutdown() override;
		void Tick(float dt) override;

		void RegisterStaticMesh(
			std::string name,
			const std::vector<vrender::render::Vertex>& vertices, 
			const std::vector<uint32_t>& indices
		);
		vrender::render::MeshHandle GetStaticMeshHandle(std::string name);
		vrender::engine::InstanceHandle CreateInstanceOfMesh(std::string name);

		vrender::engine::Transform& GetInstanceTransform(vrender::engine::InstanceHandle handle);
	private:
		vrender::engine::RenderService* render_service;
		vrender::utility::Generator<InstanceEntry> generator;

		std::unordered_map<std::string, vrender::render::MeshHandle> mesh_name_bindings;
	};
}

#endif