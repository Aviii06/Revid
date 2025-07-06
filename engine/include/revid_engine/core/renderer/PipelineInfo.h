#pragma once
#include <types/Containers.h>
#include "Renderer.h"

namespace Revid
{
	class PipelineInfo
	{
	public:
		PipelineInfo() = default;
		PipelineInfo(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		void UpdateUniformBuffer(const UniformBufferObject& obj, int imageIndex)
		{
			if (imageIndex < 0 || imageIndex >= m_uniformBuffersMapped.size())
			{
				throw std::out_of_range("Image index out of range for uniform buffer mapping.");
			}
			memcpy(m_uniformBuffersMapped[imageIndex], &obj, sizeof(UniformBufferObject));
		}

		const VkDescriptorSet* GetDescriptorSet(uint32_t index) const
		{
			if (index < m_descriptorSet.size())
			{
				return &m_descriptorSet[index];
			}
			throw std::out_of_range("Descriptor set index out of range.");
		}

		VkPipelineLayout GetPipelineLayout() const
		{
			return m_pipelineLayout;
		}

	private:
		void createDescriptorPool();
		void createDescriptorSets();
		void createPipeline();
		VkShaderModule createShaderModule(const std::vector<char>& code);

	private:
		VkDevice m_device;
		std::string m_vertexShaderPath;
		std::string m_fragmentShaderPath;

		VkPipelineLayout m_pipelineLayout;
		VkPipeline m_pipeline;
		VkDescriptorSetLayout m_descriptorSetLayout;
		VkDescriptorPool m_descriptorPool;
		Vector<VkDescriptorSet> m_descriptorSet;
		Vector<VkBuffer> m_uniformBuffers;
		Vector<VkDeviceMemory> m_uniformBuffersMemory;
		Vector<void*> m_uniformBuffersMapped;
	};
}

// namespace std
// {
// 	template <>
// 	struct hash<Revid::PipelineInfo>
// 	{
// 		size_t operator()(const Revid::PipelineInfo& info) const
// 		{
// 			return hash<string>()(info.vertexShaderPath) ^ hash<string>()(info.fragmentShaderPath);
// 		}
// 	};
// }