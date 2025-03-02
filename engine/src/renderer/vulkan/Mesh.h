#pragma once
#include <vulkan/vulkan_core.h>

#include "Vertex.h"
#include "types/Containers.h"

namespace Revid
{
	class Mesh
	{
	public:
		Mesh(String file_name);
		~Mesh();
		VkBuffer GetVertexBuffer() { return m_vertexBuffer; }
		VkBuffer GetIndexBuffer() { return m_indexBuffer; }
		int GetInstanceCount() { return m_instanceCount; }

	private:
		void createVertexBuffer();
		void createIndexBuffer();

	private:
		VkDevice m_device;
		VkBuffer m_vertexBuffer;
		VkDeviceMemory m_vertexBufferMemory;
		VkBuffer m_indexBuffer;
		VkDeviceMemory m_indexBufferMemory;

		Vector<SimpleVertex> m_vertices;
		Vector<uint16_t> m_indices;
		glm::mat4 m_modelMatrix;

		int m_instanceCount;
	};
}

