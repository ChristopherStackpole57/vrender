#include <iostream>

#include "Configuration/VulkanConfig.h"

#include <vulkan/vulkan.h>

#include <PlatformLayer/Utility/Event.h>
#include <PlatformLayer/Utility/WindowMode.h>
#include <PlatformLayer/WindowBackends/GLFWWindowBackend.h>

#include <Core/Renderer.h>
#include <Core/Mesh.h>
#include <Core/GeometryArena.h>

#include <RenderLayer/Configuration/InstanceConfiguration.h>

#ifdef NDEBUG
const bool ENABLE_VALIDATION_LAYERS = false;
#else
const bool ENABLE_VALIDATION_LAYERS = true;
#endif

int main()
{
	/*
	Create a shared pointer to a window provider, and from it generate a pointer to a surface provider.
	Because all window backends inherit from both the WindowProvider and WindowSurfaceProvider this pointer conversion is valid.
	Holding the pointer as window provider allows access to the WindowProvider API which will be interacted with in the primary
	execution loop. The surface provider, on the other hand, is necessary for generating a VkSurfaceKHR in the Instance constructor.
	*/

	std::shared_ptr<vrender::platform::WindowProvider> window_provider_ptr = std::make_shared<vrender::platform::GLFWWindowBackend>();
	std::shared_ptr<vrender::platform::WindowSurfaceProvider> surface_provider_ptr = std::dynamic_pointer_cast<
		vrender::platform::WindowSurfaceProvider
	>(window_provider_ptr);

	vrender::render::config::InstanceConfig instance_config{
		.enable_validation = ENABLE_VALIDATION_LAYERS,
	};
	//instance_config.extensions = std::vector<std::string>{};					// Not needed yet
	vrender::render::Renderer renderer(
		*window_provider_ptr,
		*surface_provider_ptr, 
		instance_config
	);

	// Add Static Meshes to Render
	vrender::render::GeometryArena& geometry_arena = renderer.get_geometry_arena();
	
	//		Star
	std::vector<vrender::render::Vertex> star_vertices = {
		{{-0.7f,  0.6f, 0}, {1,0,0}},
		{{-0.6f,  0.3f, 0}, {1,1,0}},
		{{-0.5f,  0.6f, 0}, {0,1,0}},
		{{-0.75f, 0.45f,0}, {0,1,1}},
		{{-0.45f, 0.45f,0}, {1,0,1}}
	};
	std::vector<uint32_t> star_indices = {
		0,1,2,
		0,3,1,
		2,1,4
	};
	const vrender::render::Mesh star = geometry_arena.create_static_mesh(star_vertices, star_indices);

	//		Circle
	std::vector<vrender::render::Vertex> circle_vertices;
	std::vector<uint32_t> circle_indices;

	circle_vertices.push_back({ {0.6f, 0.5f, 0}, {1,1,1} }); // center

	const int segments = 32;
	float radius = 0.2f;

	for (int i = 0; i <= segments; ++i) {
		float angle = 2.f * 3.141592 * i / segments;
		float x = 0.5f + cos(angle) * radius;
		float y = 0.5f + sin(angle) * radius;

		circle_vertices.push_back({ {x, y, 0}, {0, 0.5f + 0.5f * cos(angle), 1} });
	}
	for (int i = 1; i <= segments; ++i) {
		circle_indices.push_back(0);
		circle_indices.push_back(i);
		circle_indices.push_back(i + 1);
	}

	const vrender::render::Mesh circle = geometry_arena.create_static_mesh(circle_vertices, circle_indices);

	//		Diamond
	std::vector<vrender::render::Vertex> diamond_vertices = {
		{{-0.75f,-0.5f,0},{1,0,0}},
		{{-0.6f,-0.3f,0},{0,1,0}},
		{{-0.45f,-0.5f,0},{0,0,1}},
		{{-0.6f,-0.7f,0},{1,1,0}},
	};
	std::vector<uint32_t> diamond_indices = {
		0,1,2,
		0,2,3
	};
	const vrender::render::Mesh diamond = geometry_arena.create_static_mesh(diamond_vertices, diamond_indices);

	//		Cross Quad
	std::vector<vrender::render::Vertex> cross_vertices = {
	{{0.4f,-0.6f,0},{1,0,0}},
	{{0.8f,-0.6f,0},{0,1,0}},
	{{0.8f,-0.2f,0},{0,0,1}},
	{{0.4f,-0.2f,0},{1,1,1}},

	{{0.6f,-0.75f,0},{1,0,1}},
	{{0.65f,-0.75f,0},{0,1,1}},
	{{0.65f,-0.05f,0},{1,1,0}},
	{{0.6f,-0.05f,0},{1,0.5f,0}},
	};
	std::vector<uint32_t> cross_indices = {
		0,1,2, 0,2,3,
		4,5,6, 4,6,7
	};
	const vrender::render::Mesh cross_quad = geometry_arena.create_static_mesh(cross_vertices, cross_indices);

	//		Sprial Triangle Strip
	std::vector<vrender::render::Vertex> spiral_vertices;
	std::vector<uint32_t> spiral_indices;

	float cx = 0.0f;
	float cy = 0.0f;

	int steps = 32;

	for (int i = 0; i < steps; ++i) {
		float t = i * 0.35f;
		float r = 0.0125f * i;

		float x = cx + cos(t) * r;
		float y = cy + sin(t) * r;

		spiral_vertices.push_back({ {x,y,0},{fabs(cos(t)), fabs(sin(t)), 1.0f} });
	}

	for (int i = 0; i < steps - 2; ++i) {
		spiral_indices.push_back(i);
		spiral_indices.push_back(i + 1);
		spiral_indices.push_back(i + 2);
	}

	const vrender::render::Mesh spiral = geometry_arena.create_static_mesh(spiral_vertices, spiral_indices);

	//		Add Meshes to Renderer
	renderer.add_mesh(star);
	renderer.add_mesh(circle);
	renderer.add_mesh(diamond);
	renderer.add_mesh(cross_quad);
	renderer.add_mesh(spiral);

	// Configure Window
	window_provider_ptr->set_title("VRENDER Engine");
	window_provider_ptr->set_resizable(true);

	// Primary Exection Loop
	bool run_loop = true;
	while (run_loop)
	{
		// Check for window closure
		if (window_provider_ptr->should_close())
		{
			// Close Window
			// WindowProvider's destructor handles terminating the backend and destroying the window, so simply exit scope
			run_loop = false;
			break;
		}

		std::vector<vrender::platform::Event> events = window_provider_ptr->poll_events();
		for (vrender::platform::Event event : events)
		{
			// Process Event
			
			if (
				event.type == vrender::platform::EventType::KeyPress || 
				event.type == vrender::platform::EventType::KeyRelease
			)
			{
				// Pass Key Input Along to Input Engine
			}
			else if (
				event.type == vrender::platform::EventType::MouseButtonPress ||
				event.type == vrender::platform::EventType::MouseButtonRelease ||
				event.type == vrender::platform::EventType::MouseWheel
			)
			{
				// Pass Mouse Input Along to Input Engine
			}

			// Mutate Engine State
		}

		// Render Step
		if (!renderer.step()) return 0;

		// Clear Resized Flag
		window_provider_ptr->clear_resize_flag();
		
		//run_loop = false;
	}

	return 0;
}