#ifdef VRENDER_HAS_GLFW

#include <stdexcept>

#include "GLFWWindowBackend.h"

// Lifetime Control
vrender::platform::GLFWWindowBackend::GLFWWindowBackend()
{
	std::pair<int, int> window_size{ 480, 270 };

	// Initialize GLFW
	if (!glfwInit())
	{
		throw std::runtime_error("ERROR: GLFW Failed to Initialize");
	}
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// Create the GLFW Window
	this->glfw_window = glfwCreateWindow(window_size.first, window_size.second, "GLFW Window", nullptr, nullptr);
	if (this->glfw_window == nullptr)
	{
		throw std::runtime_error("ERROR: GLFW Failed to Create Window");
	}

	// Set some initial window state
	this->windowed_size = window_size;
	glfwGetWindowPos(this->glfw_window, &this->windowed_pos.first, &this->windowed_pos.second);

	// Register the resize callback to handle window resizing
	glfwSetWindowSizeCallback(this->glfw_window, [](GLFWwindow* window, int width, int height)
		{
			GLFWWindowBackend* backend = static_cast<GLFWWindowBackend*>(glfwGetWindowUserPointer(window));

			backend->resize_state.previous_size = std::pair<int, int>{
				backend->resize_state.size.first,
				backend->resize_state.size.second
			};

			backend->resize_state.size = std::pair<int, int>{
				width,
				height
			};

			backend->resize_state.resized = true;

			// TODO: make updating the window size here also affect the stored window size (or just make the stored window size depend on the new resize.size)
		}
	);
	glfwSetWindowUserPointer(this->glfw_window, this);
}
vrender::platform::GLFWWindowBackend::~GLFWWindowBackend()
{
	if (glfw_window != nullptr)
	{
		glfwDestroyWindow(static_cast<GLFWwindow*>(glfw_window));
	}
	glfwTerminate();
}

// WindowProvider Contract
//		Event Processing
std::vector<vrender::platform::Event> vrender::platform::GLFWWindowBackend::poll_events()
{
	this->event_queue.clear();

	glfwSetKeyCallback(this->glfw_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			vrender::platform::Event event{};
			if (action == GLFW_PRESS)
			{
				event.type = vrender::platform::EventType::KeyPress;
			}
			else if (action == GLFW_RELEASE)
			{
				event.type = vrender::platform::EventType::KeyRelease;
			}
			event.key.key_code = key;

			GLFWWindowBackend* backend = static_cast<GLFWWindowBackend*>(glfwGetWindowUserPointer(window));
			backend->event_queue.push_back(event);
		}
	);
	glfwSetMouseButtonCallback(this->glfw_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			vrender::platform::Event event{};
			if (action == GLFW_PRESS)
			{
				event.type = vrender::platform::EventType::MouseButtonPress;
				event.mouse_button.button = button;
			}
			else if (action == GLFW_RELEASE)
			{
				event.type = vrender::platform::EventType::MouseButtonRelease;
				event.mouse_button.button = button;
			}

			GLFWWindowBackend* backend = static_cast<GLFWWindowBackend*>(glfwGetWindowUserPointer(window));
			backend->event_queue.push_back(event);
		}
	);

	glfwPollEvents();

	// Reset Resized State Flag
	this->resize_state.resized = false;

	return event_queue;
}
std::vector<vrender::platform::Event> vrender::platform::GLFWWindowBackend::wait_events()
{
	// TODO: Implement
	std::vector<vrender::platform::Event> event_queue;

	return event_queue;
}
bool vrender::platform::GLFWWindowBackend::should_close()
{
	return glfwWindowShouldClose(this->glfw_window);
}

//		Window State Queries
std::pair<int, int> vrender::platform::GLFWWindowBackend::get_window_size() const
{
	int width = 0, height = 0;
	glfwGetWindowSize(this->glfw_window, &width, &height);

	return std::pair<int, int>(width, height);
}
std::pair<int, int> vrender::platform::GLFWWindowBackend::get_framebuffer_size() const
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(this->glfw_window, &width, &height);

	return std::pair<int, int>(width, height);
}
bool vrender::platform::GLFWWindowBackend::is_minimized() const
{
	int minimized = glfwGetWindowAttrib(this->glfw_window, GLFW_ICONIFIED);
	return (bool)minimized;
}
bool vrender::platform::GLFWWindowBackend::is_focused() const
{
	int focused = glfwGetWindowAttrib(this->glfw_window, GLFW_FOCUSED);
	return (bool)focused;
}

//		Window State Mutation
void vrender::platform::GLFWWindowBackend::set_title(const std::string& title)
{
	glfwSetWindowTitle(this->glfw_window, title.c_str());
}
void vrender::platform::GLFWWindowBackend::set_window_mode(const vrender::platform::WindowMode mode)
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* glfw_vidmode = glfwGetVideoMode(monitor);

	switch (mode)
	{
	case vrender::platform::WindowMode::Windowed:
		glfwSetWindowAttrib(this->glfw_window, GLFW_DECORATED, GLFW_TRUE);
		glfwSetWindowMonitor(
			this->glfw_window,
			nullptr,
			this->windowed_pos.first,
			this->windowed_pos.second,
			this->windowed_size.first,
			this->windowed_size.second,
			GLFW_DONT_CARE
		);
		break;
	case vrender::platform::WindowMode::Borderless:
		glfwSetWindowAttrib(this->glfw_window, GLFW_DECORATED, GLFW_FALSE);
		glfwSetWindowMonitor(
			this->glfw_window,
			nullptr,
			0,
			0,
			glfw_vidmode->width,
			glfw_vidmode->height,
			GLFW_DONT_CARE
		);
		break;
	case vrender::platform::WindowMode::Fullscreen:
		glfwSetWindowMonitor(
			this->glfw_window,
			monitor,
			0,
			0,
			glfw_vidmode->width,
			glfw_vidmode->height,
			glfw_vidmode->refreshRate
		);
		break;
	}
}
void vrender::platform::GLFWWindowBackend::set_resizable(const bool resizable)
{
	glfwSetWindowAttrib(this->glfw_window, GLFW_RESIZABLE, resizable);
}
void vrender::platform::GLFWWindowBackend::set_visible(const bool visible)
{
	glfwSetWindowAttrib(this->glfw_window, GLFW_VISIBLE, visible);
}

//		Resize Signaling
bool vrender::platform::GLFWWindowBackend::was_resized() const
{
	return resize_state.resized;
}
std::pair<int, int> vrender::platform::GLFWWindowBackend::get_last_resize_size() const
{
	return resize_state.previous_size;
}

// WindowSurfaceProvider Contract
std::vector<std::string> vrender::platform::GLFWWindowBackend::get_required_instance_extensions() const
{
	uint32_t glfw_extension_count = 0;
	const char** raw_extension_pointers = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

	std::vector<std::string> extensions;
	extensions.reserve(glfw_extension_count);

	for (int i = 0; i < glfw_extension_count; i++)
	{
		extensions.emplace_back(raw_extension_pointers[i]);
	}

	return extensions;
}
VkSurfaceKHR vrender::platform::GLFWWindowBackend::create_surface(VkInstance instance) const
{
	VkSurfaceKHR surface;
	VkResult surface_creation_result = glfwCreateWindowSurface(
		instance,
		static_cast<GLFWwindow*>(glfw_window),
		nullptr,
		&surface
	);

	if (surface_creation_result != VK_SUCCESS)
	{
		throw std::runtime_error("ERROR: GLFW Window Failed to Create Vulkan Surface");
	}

	return surface;
}
bool vrender::platform::GLFWWindowBackend::destroy_surface(VkInstance instance, VkSurfaceKHR surface) const
{
	if (surface == VK_NULL_HANDLE)
	{
		return true;
	}

	vkDestroySurfaceKHR(instance, surface, nullptr);
	if (surface == VK_NULL_HANDLE)
	{
		return true;
	}
	return false;
}

#endif