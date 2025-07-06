#pragma once

#pragma once
#include <revid_engine/core/renderer/RenderGraph.h>
#include <revid_engine/core/renderer/IRenderStage.h>

namespace Revid
{
    class RenderGraphBuilder
    {
    public:
        RenderGraphBuilder& BeginPass(const std::string& name, VkExtent2D extent)
        {
            m_passes.push_back(RenderPassTemplate{.name = name, .extent = extent});
            m_currentPass = &m_passes.back();
            return *this;
        }

        RenderGraphBuilder& AddAttachment(const std::string& name, VkFormat format, bool clear, bool isDepth)
        {
            m_currentPass->attachments.push_back(AttachmentTemplate{name, format, clear, isDepth});
            return *this;
        }

        RenderGraphBuilder& AddSwapchainAttachment(const std::string& name, VkFormat format)
        {
            m_currentPass->attachments.push_back(AttachmentTemplate{name, format, true, false, true});
            return *this;
        }

        RenderGraphBuilder& AddSubpass(uint32_t index,
                                       const std::vector<std::string>& colorAttachments,
                                       const std::string& depthAttachment = "",
                                       bool readFromPrevious = false)
        {
            SubpassTemplate subpass;
            subpass.index = index;
            subpass.colorAttachments = colorAttachments;
            if (!depthAttachment.empty())
                subpass.depthAttachment = depthAttachment;
            subpass.readFromPrevious = readFromPrevious;
            m_currentPass->subpasses.push_back(subpass);
            return *this;
        }

        RenderGraphBuilder& EndPass()
        {
            m_currentPass = nullptr;
            return *this;
        }

        void Build(RenderGraph& graph, VkDevice device, const std::vector<VkImageView>& swapchainImageViews)
        {
            for (const auto& passTemplate : m_passes)
            {
                graph.CreatePassFromTemplate(passTemplate, device, swapchainImageViews);
            }
        }

    private:
        std::vector<RenderPassTemplate> m_passes;
        RenderPassTemplate* m_currentPass = nullptr;
    };
}

