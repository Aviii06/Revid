#pragma once
#include <types/Containers.h>
#include <revid_engine/core/renderer/RenderGraphTemplates.h>
#include <revid_engine/core/renderer/RenderCommand.h>

namespace Revid
{
    struct RenderPassNode
    {
        std::string name;
        VkRenderPass renderPass;
        VkExtent2D extent;
        std::vector<AttachmentTemplate> attachments;
        std::vector<VkImageView> attachmentViews;
        std::vector<VkFramebuffer> framebuffers;

        uint32_t subpassCount = 0;
        std::vector<std::unordered_map<uint32_t, std::vector<RenderCommand>>> commandsPerFrame;

        void Initialize(uint32_t framesInFlight)
        {
            commandsPerFrame.resize(framesInFlight);
        }
    };

    class RenderGraph
    {
    public:
        void CreatePassFromTemplate(const RenderPassTemplate& tmpl,
                                    VkDevice device,
                                    const std::vector<VkImageView>& swapchainViews);

        void SubmitCommand(const std::string& passName,
                           uint32_t subpass,
                           uint32_t frameIndex,
                           const RenderCommand& cmd);

        void ExecuteAllCommands(VkCommandBuffer cmd, uint32_t frameIndex);

    private:
        std::unordered_map<std::string, Ptr<RenderPassNode>> m_passes;
    };
}
