cmake_minimum_required(VERSION 3.20)

project(vrender LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(VRENDER_ROOT_DIR ${CMAKE_SOURCE_DIR}/src)

add_library(vrender
"src/PlatformLayer/WindowBackends/GLFWWindowBackend.cpp"
"src/PlatformLayer/WindowBackends/SDLWindowBackend.cpp"
"src/PlatformLayer/WindowBackends/SFMLWindowBackend.cpp"

"src/RenderLayer/Instance.cpp"
"src/RenderLayer/Renderer.cpp"
"src/RenderLayer/PhysicalDevice.cpp" 
"src/RenderLayer/LogicalDevice.cpp" 
"src/RenderLayer/Swapchain.cpp" 
"src/RenderLayer/Semaphore.cpp" 
"src/RenderLayer/Framebuffer.cpp" 
"src/RenderLayer/RenderPass.cpp"

"src/RenderLayer/Utility/PhysicalDeviceEnumeration.cpp" 
"src/RenderLayer/Utility/SelectPhysicalDevice.cpp"
"src/RenderLayer/Utility/QueueHandle.h"
"src/RenderLayer/Utility/QueueSelection.cpp" 
"src/RenderLayer/Utility/QuerySwapchainSupport.cpp" 
"src/RenderLayer/Utility/ConfigureSwapchainCreateInfo.cpp"  )

find_package(Vulkan REQUIRED MODULE)
target_link_libraries(vrender PUBLIC Vulkan::Vulkan)

# Safely look for GLFW's existence
set(GLFW_DIR ${CMAKE_SOURCE_DIR}/external/glfw-3.4)
if (EXISTS ${GLFW_DIR}/CMakeLists.txt)
	message(STATUS "BUILD: GLFW found, enabling backend")

	add_subdirectory(${GLFW_DIR})

	target_include_directories(vrender PUBLIC ${GLFW_DIR}/include)
	target_link_libraries(vrender PUBLIC glfw)
	target_compile_definitions(vrender PUBLIC VRENDER_HAS_GLFW)
else()
	message(STATUS "BUILD: GLFW not found, disabling backend")
endif()

# Safely look for SDL's existence
set(SDL_DIR ${CMAKE_SOURCE_DIR}/external/sdl-3.4)
if (EXISTS ${SDL_DIR}/CMakeLists.txt)
	message(STATUS "BUILD: SDL found, enabling backend")

	add_subdirectory(${SDL_DIR})

	target_include_directories(vrender PUBLIC ${SDL_DIR}/include)
	target_link_libraries(vrender PUBLIC SDL3::SDL3)
	target_compile_definitions(vrender PUBLIC VRENDER_HAS_SDL)
else()
	message(STATUS "BUILD: SDL not found, disabling backend")
endif()

# Safely look for SFML's existence
set(SFML_DIR ${CMAKE_SOURCE_DIR}/external/sfml-3.0)
if (EXISTS ${SFML_DIR}/CMakeLists.txt)
	message(STATUS "BUILD: SFML found, enabling backend")

	add_subdirectory(${SFML_DIR})

	target_include_directories(vrender PUBLIC ${SFML_DIR}/include)
	target_link_libraries(vrender PUBLIC 
		SFML::System
		SFML::Window
		SFML::Graphics
	)
	target_compile_definitions(vrender PUBLIC VRENDER_HAS_SFML)
else()
	message(STATUS "BUILD: SFML not found, disabling backend")
endif()

# Safely Look for WIN32's existence
if (WIN32)
	message(STATUS "BUILD: WIN32 found, enabling backend")

	target_compile_definitions(vrender PUBLIC VRENDER_HAS_WIN32)
else()
	message(STATUS "BUILD: WIN32 not found, disabling backend")
endif()

target_include_directories(vrender PUBLIC
	src/
	src/PlatformLayer/WindowBackends/
	include/
)

add_executable(vrender_test main.cpp 
"src/RenderLayer/Renderer.cpp" 
"src/RenderLayer/PhysicalDevice.cpp" 
"src/RenderLayer/LogicalDevice.cpp" 
"src/RenderLayer/Utility/PhysicalDeviceEnumeration.cpp" 
"src/RenderLayer/Utility/SelectPhysicalDevice.cpp" 
"src/RenderLayer/Utility/QueueSelection.cpp" 
"src/RenderLayer/Swapchain.cpp" 
"src/RenderLayer/Utility/QuerySwapchainSupport.cpp" 
"src/RenderLayer/Utility/ConfigureSwapchainCreateInfo.cpp" 
"src/RenderLayer/Semaphore.cpp" 
"src/RenderLayer/Framebuffer.cpp" 
"src/RenderLayer/RenderPass.cpp")
target_link_libraries(vrender_test PRIVATE vrender)