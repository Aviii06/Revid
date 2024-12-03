#pragma once
#include "types/Containers.h"
#include "Buffer.h"
#include "Vertex.h"

namespace Revid
{
    class VertexBuffer
    {
    public:
		VertexBuffer();

		VertexBuffer(const Vector<SimpleVertex>& verts);

		void SetVertices(const Vector<SimpleVertex>& verts);

		Vector<VkBuffer> GetVulkanBuffer() { return m_vertexBuffers; }

    private:
	    VkDevice m_device;

		void createVertexBuffer();

    private:
		Buffer m_bf;
        Vector<VkBuffer> m_vertexBuffers;
        size_t m_vertexCount;
        VkDeviceMemory m_vertexBufferMemory;

        Vector<SimpleVertex> m_vertices;
        // Vector<T> m_vertices = {
        //    {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
        //  {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        //  {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        //  {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},

        //    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        //  {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        //  {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},
        //  {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}}
        //};
    };
}
