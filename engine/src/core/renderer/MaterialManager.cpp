#include "revid_engine/core/renderer/MaterialManager.h"

uint32_t Revid::MaterialManager::AddMaterial(const Material& material)
{
	uint32_t id = m_nextID++;
	m_materials[id] = material;

	const auto& pipelineBundle = m_pipelineManager->GetOrCreatePipeline(material.pipelineInfo);
	auto layout = pipelineBundle.descriptorSetLayouts[0];

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_pool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &layout;

	VkDescriptorSet descriptorSet;
	vkAllocateDescriptorSets(m_device, &allocInfo, &descriptorSet);

	m_descriptorSets[id] = descriptorSet;
	return id;
}
