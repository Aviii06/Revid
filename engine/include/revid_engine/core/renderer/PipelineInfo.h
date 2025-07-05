#pragma once
#include <types/Containers.h>
#include <vulkan/vulkan.h>

namespace Revid
{
	struct PipelineInfo
	{
		std::string vertexShaderPath;
		std::string fragmentShaderPath;

		VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;

		std::vector<VkPushConstantRange> pushConstants;
		std::vector<VkDescriptorSetLayoutBinding> descriptorBindings;

		VkRenderPass renderPass = VK_NULL_HANDLE;
		uint32_t subpass = 0;
		uint32_t numberOfColorAttachments = 1;

		bool operator==(const PipelineInfo& other) const
		{
			return vertexShaderPath == other.vertexShaderPath &&
				   fragmentShaderPath == other.fragmentShaderPath &&
				   topology == other.topology &&
				   polygonMode == other.polygonMode &&
				   renderPass == other.renderPass &&  // Required for G-buffer/lighting/final passes
				   subpass == other.subpass;
		}
	};
}

namespace std
{
	template <>
	struct hash<Revid::PipelineInfo>
	{
		size_t operator()(const Revid::PipelineInfo& info) const
		{
			return hash<string>()(info.vertexShaderPath) ^ hash<string>()(info.fragmentShaderPath);
		}
	};
}