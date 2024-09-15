#pragma once
#include "maths/Vec.h"
#include "types/Containers.h"

namespace Revid
{
    struct Vertex
    {
        Maths::Vec2 m_position;
        Maths::Vec3 m_color;

        static Vector<VkVertexInputBindingDescription> GetBindingDescriptions()
        {
            Vector<VkVertexInputBindingDescription> bindingDescriptions(
            1,
                VkVertexInputBindingDescription{}
            );
            bindingDescriptions[0].binding = 0;
            bindingDescriptions[0].stride = sizeof(Vertex);
            bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescriptions;
        }


        static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, m_position);


            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, m_color);

            return attributeDescriptions;
        }
    };
}