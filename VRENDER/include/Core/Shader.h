#ifndef RENDER_SHADER_H
#define RENDER_SHADER_H

#include <iostream>

#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <filesystem>

#include <vulkan/vulkan.h>

#include <Core/LogicalDevice.h>

namespace vrender::render
{
	class Shader
	{
	public:
		// Lifetime Control
		Shader(const vrender::render::LogicalDevice& logical_device, const std::string shader_path);
		//Shader(const vrender::render::LogicalDevice& logical_device, const uint32_t* shader_source);
		~Shader();

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		Shader(Shader&& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;

		// API Accessibility
		VkShaderModule get_shader_module() const;
		const std::string& get_shader_path() const;
		uint32_t get_bytecode_size() const;
	private:
		VkShaderModule shader = VK_NULL_HANDLE;
		const vrender::render::LogicalDevice* logical_device_ptr = nullptr;

		std::string shader_path;
		uint32_t bytecode_size = 0;
	};
}

#endif