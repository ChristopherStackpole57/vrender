#include <Shader.h>

// Utility Functions
static std::vector<uint32_t> read_shader_source(const std::string& path)
{
	std::ifstream file(path, std::ios::ate | std::ios::binary);
	if (!file.is_open())
	{
		throw std::runtime_error("ERROR: VRENDER Unable to Open Shader Source: " + path);
	}

	// NOTE: SPIR-V Files Must be 4-Byte Alligned
	size_t file_size = static_cast<size_t>(file.tellg());
	if (file_size % 4 != 0)
	{
		throw std::runtime_error("ERROR: Shader Source: " + path + " is Not Four Byte Aligned");
	}

	std::vector<uint32_t> shader_source(file_size / 4);

	file.seekg(0);
	file.read(reinterpret_cast<char*>(shader_source.data()), file_size);

	file.close();

	return shader_source;
}

// Lifetime Control
vrender::render::Shader::Shader(
	const vrender::render::LogicalDevice& logical_device,
	const std::string shader_path
)
	: logical_device_ptr(&logical_device)
	, shader_path(shader_path)
{
	std::vector<uint32_t> shader_source = read_shader_source(shader_path);
	this->bytecode_size = shader_source.size() * sizeof(uint32_t);

	VkShaderModuleCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	create_info.pNext = nullptr;
	create_info.flags = 0;
	create_info.codeSize = this->bytecode_size;
	create_info.pCode = shader_source.data();

	VkResult creation_result = vkCreateShaderModule(
		logical_device.get_logical_device(),
		&create_info,
		nullptr,
		&this->shader
	);
	if (creation_result != VK_SUCCESS)
	{
		std::cout << creation_result << std::endl;
		throw std::runtime_error("ERROR: Vulkan Failed to Create Shader Module");
	}
}
/*vrender::render::Shader::Shader(
	const vrender::render::LogicalDevice& logical_device,
	const uint32_t* shader_source
)
	: logical_device_ptr(&logical_device)
{

}*/ 
vrender::render::Shader::~Shader()
{
	if (this->shader == VK_NULL_HANDLE)
	{
		return;
	}

	vkDestroyShaderModule(
		this->logical_device_ptr->get_logical_device(),
		this->shader,
		nullptr
	);

	this->shader = VK_NULL_HANDLE;
	this->logical_device_ptr = nullptr;
}

vrender::render::Shader::Shader(vrender::render::Shader&& other) noexcept
	: shader(other.shader)
	, logical_device_ptr(other.logical_device_ptr)
	, shader_path(other.shader_path)
	, bytecode_size(other.bytecode_size)
{
	other.shader = VK_NULL_HANDLE;
	other.logical_device_ptr = nullptr;
}
vrender::render::Shader& vrender::render::Shader::operator=(vrender::render::Shader&& other) noexcept
{
	if (this != &other)
	{
		this->shader = other.shader;
		this->logical_device_ptr = other.logical_device_ptr;
		this->shader_path = other.shader_path;
		this->bytecode_size = other.bytecode_size;

		other.shader = VK_NULL_HANDLE;
		other.logical_device_ptr = nullptr;
	}

	return *this;
}

// API Accessibility
VkShaderModule vrender::render::Shader::get_shader_module() const
{
	return this->shader;
}
const std::string& vrender::render::Shader::get_shader_path() const
{
	return this->shader_path;
}
uint32_t vrender::render::Shader::get_bytecode_size() const
{
	return this->bytecode_size;
}