#pragma once
#include <types/Containers.h>
#include <types/SmartPointers.h>
#include <revid_engine/core/renderer/RenderGraphTemplates.h>

namespace Revid
{
    using RenderCommand = std::function<void(VkCommandBuffer, uint32_t)>;

    struct RenderPassNode
    {
        std::string name;
        VkRenderPass renderPass;
        VkExtent2D extent;
        Vector<AttachmentTemplate> attachments;
        Vector<VkImageView> attachmentViews;
        Vector<VkFramebuffer> framebuffers;

        uint32_t subpassCount = 0;
        UnorderedMap<uint32_t, Vector<RenderCommand>> commandsPerFrame;
    };

    class RenderGraph
    {
    public:
        void AddPassFromTemplate(const RenderPassTemplate& tmpl,
                                 VkDevice device,
                                 const std::vector<VkImageView>& swapchainViews);

        void SubmitCommand(uint32_t passIndex,
                           uint32_t subpass,
                           const RenderCommand& cmd);

        void ExecuteAllCommands(VkCommandBuffer& cmd, uint32_t frameIndex);

    private:
        Vector<Ptr<RenderPassNode>> m_passes;
    };
}
