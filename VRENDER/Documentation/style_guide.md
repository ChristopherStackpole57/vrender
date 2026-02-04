# Writing Vulkan Every Day Until I Have A Rendering Engine
## Style Guide

#### 1. File & Directory Naming
- Directories: **PascalCase** (`CoreSystems/`, `Utilities/`)
- Files: **PascalCase** (`VulkanConfig.h`, `PhysicalDevice.h`)
- Markdown Files: **snake_case** (`style_guide.md`, `roadmap.md`)
- Header & source share base name: `Instance.h` - `Instance.cpp`
- One class/abstraction per file
- Avoid abbreviations unless standard (`Vk` is fine)

#### 2. Classes & Structs
- **PascalCase**: `CommandPool`, `LogicalDevice`
- Do not keep the Vk prefix in any publicly exposed cases
	- Within the confines of a translation unit Vk prefixes can be maintained as deemed necessary or useful
- Use descriptive names, slight verbosity is acceptable: `CommandPoolAndBuffer`

#### 3. Functions / Methods
- **snake_case**: `create_swapchain()`, `submit_command_buffer()`
- Verbs for actions, get/set prefix for accessors: `create_window()`, `get_surface_target()`

#### 4. Variables
- **snake_case** for locals: `current_frame`, `image_count`
- **UPPER_SNAKE_CASE** for constants, macros, etc.: `MAX_FRAMES_IN_FLIGHT`
- Optional pointer clarity: `Device* device_ptr`

#### 5. Member Variables
- **snake_case**: `glfw_context`, `command_pool`
- Keep private unless necessary

#### 6. Namespaces
- **lowercase**: `platform`, `filesystem`
- Top-level: `vrender`
- Sub-namespaces for systems: `vrender::renderapp`, `vrender::utility`
- Avoid global namespace

#### 7. Enums
- Enum types: **PascalCase**: `FrameState`, `ShaderStage`
- Enum values: **UPPER_SNAKE_CASE**: `FRAME_READY`, `FRAME_SUBMITTED`
- Prefer `enum class` over anonymous enums

#### 8. Macros / Preprocessor
- Avoid macros unless necessary
- All caps with underscores: `VK_USE_PLATFORM_WIN32_KHR`
- Use `constexpr` instead of `#define` constants

#### 9. Formatting & Indentation
- Tabs instead of spaces
- Curly braces on new line
- One statement per line
- Space around operators and commas: `int result = (some_function(4, 3) - 2) / 10;`
- Some verbosity for each line is acceptable, however, rarely should lines need to exceed more than 120 characters

#### 10. Comments & Documentation
- `//` for inline, `/* */` for long blocks
- Document **ownership**, **lifetimes**, and **Vulkan quirks**
- Explain *why*, not *what*

#### 11. Git / Commit Practices
- One commit per logical change or (more likely) per day
- Commit messages: `[Subsystem] Day X: Short description`
- Link commits to daily challenge posts if relevant
- It is preferred that commit descriptions carry a little more information about what specifically was added