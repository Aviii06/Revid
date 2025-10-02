#pragma once
#include <types/Containers.h>
#include <vulkan/vulkan.h>

namespace Revid
{
    struct AttachmentTemplate
    {
        String name;
        VkFormat format;
        bool clear;
        bool isDepth = false;
        bool isSwapchain = false;
    };

    struct AttachmentRefTemplate
    {
        String name;
        VkImageLayout layout;
    };

    struct SubpassTemplate
    {
        uint32_t index;
        Vector<AttachmentRefTemplate> colorAttachments;
        Vector<AttachmentRefTemplate> inputAttachments;
        std::optional<String> depthAttachment;
        bool readFromPrevious = false;
    };

    struct RenderPassTemplate
    {
        String name;
        VkExtent2D extent;
        Vector<AttachmentTemplate> attachments;
        Vector<SubpassTemplate> subpasses;
    };

    struct AllocatedImage
    {
        VkImage image;
        VkDeviceMemory memory;
        VkImageView view;
    };
}
