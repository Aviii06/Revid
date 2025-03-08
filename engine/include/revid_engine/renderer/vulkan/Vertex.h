#pragma once
#include <vulkan/vulkan.h>

#include "maths/Vec.h"
#include "revid_engine/renderer/vulkan/utils/VertexUtils.h"
#include <array>

namespace Revid
{
    struct SimpleVertex
    {
        Maths::Vec3 m_position;
        // Maths::Vec3 m_normal;
        // Maths::Vec3 m_tangent;
        Maths::Vec3 m_color;
        // Maths::Vec2 m_texcoord;

        static VkVertexInputBindingDescription GetBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(SimpleVertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }


        static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(SimpleVertex, m_position);


            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(SimpleVertex, m_color);

            return attributeDescriptions;
        }
    };
}
