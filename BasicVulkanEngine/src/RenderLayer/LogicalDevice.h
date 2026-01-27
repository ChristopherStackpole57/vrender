#ifndef RENDER_LOGICAL_DEVICE_H
#define RENDER_LOGICAL_DEVICE_H

#include <cassert>
#include <set>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

#include <RenderLayer/PhysicalDevice.h>
#include <RenderLayer/Utility/QueueSelection.h>
#include <RenderLayer/Utility/QueueHandle.h>

namespace vrender::render
{
	class LogicalDevice
	{
	public:
		// Lifetime Control
		LogicalDevice(
			const vrender::render::PhysicalDevice& physical_device,
			const vrender::render::utility::queue::QueueSelection& queue_selection_result	
		);
		~LogicalDevice();

		// API Accessibility
		const VkDevice get_logical_device() const;
		const VkQueue get_graphics_queue() const;
		const VkQueue get_compute_queue() const;
		const std::vector<std::string>& get_enabled_extensions() const;
	private:
		VkDevice device;
		std::vector<vrender::render::utility::queue::QueueHandle> queues;
		std::vector<std::string> enabled_extensions;
	};
}

#endif