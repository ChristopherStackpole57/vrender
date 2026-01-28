# Writing Vulkan Every Day Until I Have A Rendering Engine
### Christopher Stackpole

## Project Status
#### Some Quick Informational Tidbits:
What This Is:

This project is a *learning* experience first and foremost. As a student of computer graphics I have a fair bit of knowledge of the
graphics pipeline; that being said, I am **not** at an industry level for *Vulkan* yet. This project is intended to remedy that. As
well, this project is meant to serve as somewhat of a living cover letter; a thorough documentation of engineering decisions that give
a glimpse into who I am as an engineer, and how I think, for those that may be interested in.

What This Is Not:

This project is **not** a step by step guide on how to make a Vulkan rendering engine. I am not a professional engineer yet, and while
I will be planning every step along the way and researching design choices as much as I can, I fully expect to make mistakes along 
the way. Mistakes are a necessary part of the learning process, and making them will give clear insight into the poor choices that
led to their arrival, and what steps can be taken in the future to prevent them.

This project is **not** my number one focus. Being a student I still have many responsibities to school and other engagements and so 
there will be days where the work I am able to output is lower. Regardless, I will be making a best effort to output incremental work.

Who This Is For:

Though a tough balance to strike, I am trying to make this project accessible to everyone. Technical enough that industry experts
can understand what is happening well enough to hopefully provide me invaluable critiques. All the while being general enough that
any non technical people can understand what is happening. 

#### A Rough Outline of this Week's Schedule:
Week 1:
- [x] Project Setup
- [x] Window Creation
- [x] Basic Validation and Debugging
- [x] Instance
- [x] PhysicalDevice
- [x] LogicalDevice

For a complete picture, view the full roadmap here:
[Full Roadmap](BasicVulkanEngine/Documentation/roadmap.md)

## Project Structure
Core Layers, dependencies flow in order of appearance:
- Platform Layer
	- The layer that binds and communicates with the OS; the Platform Layer is responsible for hiding platform details from the other layers and ensuring that code nowhere else in the codebase is reliant on specific platform details
- Render Layer
	- The layer through which most all Vulkan code is operated; the Render Layer is what makes this an 'engine' and not just some OS interactions

Upper-Level Layers:
- *TBD*

Project Dependencies:
- Vulkan SDK
- GLFW 3.x

## Design Principles
Lifetime and Ownership:
- Vulkan wrapper classes should use RAII principles for clean memory management
- Ownership is unique whenever possible

Abstraction Boundaries:
- Layer Responsibilities
	- Render Layer
		- *Owns*: the Vulkan instance and all related Vulkan code
		- *Exposes*: wrappers for common Vulkan tools and uses (e.g frames, passes, pass ordering, commands to draw, etc.)
	- Platform Layer
		- *Owns*: the window from the OS, filestream communication, and surface creation
		- *Exposes*: a VkSurfaceKHR for use in the Render Layer, as well as common information and events like monitor details, resize events, drawable extents, etc.
- Dependency Direction
	- Dependencies must flow **unidirectionally** in terms of 'abstractness'; more concretely, lower level code must not be dependent on higher level code
	- As a concrete example, the dependency chain may look like: Platform -> Surface -> Renderer -> Rendering Engine
	- In this example, the platform code (such as Win32, GLFW, SDL, SFML, etc.) must not rely on anything from the higher level Renderer code
- Vulkan Handle Usage
	- As much as possible, Vulkan handles should be kept within the Render Layer or the Platform Layer (in the case of VkSurfaceKHR)
	- Code at the highest levels (e.g. render code that could be integrated in a real-time simulation model, a game engine, rendering software, etc.) must never work directly with raw Vulkan handles

Clarity vs. Performance:
- In almost all cases clarity is preferred over performance
- **Except** where specific bottlenecks have been measured in the codebase; in these locations performance is preferred

## Naming Conventions

[Style Guide](BasicVulkanEngine/Documentation/style_guide.md)

## Build
Target Platform: (*currently*) only Windows
Required Dependencies: Vulkan, GLFW
Limitations: TBD
No Rendering Output Yet