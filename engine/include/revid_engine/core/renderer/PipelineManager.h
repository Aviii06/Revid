#pragma once
#include <vulkan/vulkan.h>
#include <types/Containers.h>
#include "PipelineInfo.h"

namespace Revid
{
    struct PipelineBundle
    {
        VkPipeline pipeline;
        VkPipelineLayout layout;
        Vector<VkDescriptorSetLayout> descriptorSetLayouts;
    };

    class PipelineManager
    {
    public:
        void Init(VkDevice device);
        const PipelineBundle& GetOrCreatePipeline(const PipelineInfo& info);
        void Cleanup();

    private:
        VkDevice m_device;
        UnorderedMap<PipelineInfo, PipelineBundle> m_pipelineCache;

        PipelineBundle CreatePipeline(const PipelineInfo& info);
    };
}

