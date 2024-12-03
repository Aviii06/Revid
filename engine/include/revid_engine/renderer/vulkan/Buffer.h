#pragma once
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>

namespace Revid
{
    class Buffer
    {
    public:
        Buffer();
        void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                          VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    private:
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    private:
        VkDevice m_device;
        VkCommandPool m_commandPool;
        VkPhysicalDevice m_physicalDevice;
        VkQueue m_graphicsQueue;
    };
}
