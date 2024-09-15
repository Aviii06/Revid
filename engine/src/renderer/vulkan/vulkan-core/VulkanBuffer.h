#pragma once

#include "VulkanDevice.h"
#include "renderer/vulkan/Vertex.h"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE  // open gl uses -1 to 1, vk is 0 to 1
#include <glm/glm.hpp>

// std
#include <memory>
#include <vector>

namespace Revid {

    class VulkanBuffer {
    public:
        VulkanBuffer(VulkanDevice&);
        ~VulkanBuffer() { cleanup(); }

        VulkanBuffer(const VulkanBuffer&) = delete;
        void operator=(const VulkanBuffer&) = delete;

        void draw(VkCommandBuffer commandBuffer);
        void bind(VkCommandBuffer commandBuffer);

    private:
        VulkanDevice& m_device;

        VkBuffer m_vertexBuffer;
        VkDeviceMemory m_vertexBufferMemory;
        uint32_t m_vertexCount;
        std::vector<Vertex> m_vertices = {
            {{0.0f, -0.5f}, {1.0, 0.0 ,0.0}},
            {{0.5f, 0.5f}, {0.0, 1.0, 0.0}},
            {{-0.5f, 0.5f}, {0.0, 0.0, 1.0}}
        };
        void createVertexBuffer();
        void cleanup();
    };
}  // namespace lve