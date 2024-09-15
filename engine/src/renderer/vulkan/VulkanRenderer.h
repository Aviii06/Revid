#pragma once
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include "renderer/vulkan/vulkan-core/VulkanBuffer.h"
#include "renderer/vulkan/vulkan-core/VulkanDevice.h"
#include "renderer/vulkan/vulkan-core/VulkanPipeline.h"
#include "renderer/vulkan/vulkan-core/VulkanSwapchain.h"

#include "types/SmartPointers.h"
#include "types/Containers.h"


namespace Revid
{
    class VulkanRenderer : public Renderer
    {
    public:
        void Init(const RendererSettings&) override;
        void Shutdown() override;
        void Render() override;

    private:
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();

    private:
        VulkanDevice m_device;
        RendererSettings m_rendererSettings;
        Ptr<VulkanSwapchain> m_swapchain = nullptr;
        VkPipelineLayout m_pipelineLayout;
        Ptr<VulkanPipeline> m_simplePipeline;
        Vector<VkCommandBuffer> m_commandBuffers;
    };
}
