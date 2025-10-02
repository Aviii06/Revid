#pragma once
#include "Material.h"
#include "PipelineManager.h"
#include <unordered_map>

namespace Revid
{
    class MaterialManager
    {
    public:
        void Init(VkDevice device, VkDescriptorPool descriptorPool, PipelineManager* pipelineManager);
        uint32_t AddMaterial(const Material& material);
        VkDescriptorSet GetDescriptorSet(uint32_t materialID);

    private:
        VkDevice m_device;
        VkDescriptorPool m_pool;
        PipelineManager* m_pipelineManager;

        std::unordered_map<uint32_t, Material> m_materials;
        std::unordered_map<uint32_t, VkDescriptorSet> m_descriptorSets;

        uint32_t m_nextID = 1;
    };
}
