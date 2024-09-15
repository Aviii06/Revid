#pragma once

#include "VulkanDevice.h"

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
        struct Vertex {
            glm::vec2 position;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions() {
                std::vector<VkVertexInputBindingDescription> bindingDescriptions(
                    1,
                    VkVertexInputBindingDescription{});

                bindingDescriptions[0].binding = 0;
                bindingDescriptions[0].stride = sizeof(Vertex);
                bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
                return bindingDescriptions;
            }

            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        VulkanBuffer(VulkanDevice& device);
        ~VulkanBuffer() { cleanup(); }

        VulkanBuffer(const VulkanBuffer&) = delete;
        void operator=(const VulkanBuffer&) = delete;

        void draw(VkCommandBuffer commandBuffer);
        void bind(VkCommandBuffer commandBuffer);

    private:
        VulkanDevice& m_device;

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
        std::vector<Vertex> vertices = {{{0.0f, -0.5f}}, {{0.5f, 0.5f}}, {{-0.5f, 0.5f}}};

        void createVertexBuffer();
        void cleanup();
    };
}  // namespace lve