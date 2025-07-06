#pragma once

#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include "RenderCommand.h"

namespace Revid
{
	class IRenderStage
	{
	public:
		virtual ~IRenderStage() = default;

		// Called each frame before Vulkan draw to gather ECS data
		virtual void Record(class Registry* registry, uint32_t frameIndex) = 0;

		// Called during command buffer recording
		virtual void Execute(VkCommandBuffer commandBuffer, uint32_t frameIndex) = 0;
	};
}
