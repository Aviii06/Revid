#pragma once
#include <types/Containers.h>

namespace Revid
{
    struct AttachmentTemplate
    {
        std::string name;
        VkFormat format;
        bool clear;
        bool isDepth;
        bool isSwapchain = false;
    };

    struct SubpassTemplate
    {
        uint32_t index;
        std::vector<std::string> colorAttachments;
        std::optional<std::string> depthAttachment;
        bool readFromPrevious = false;
    };

    struct RenderPassTemplate
    {
        std::string name;
        VkExtent2D extent;
        std::vector<AttachmentTemplate> attachments;
        std::vector<SubpassTemplate> subpasses;
    };

    struct AllocatedImage
    {
        VkImage image;
        VkDeviceMemory memory;
        VkImageView view;
    };
}
