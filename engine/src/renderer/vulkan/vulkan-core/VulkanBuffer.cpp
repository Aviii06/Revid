#include "VulkanBuffer.h"

// lib headers
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

// std
#include <cassert>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

namespace Revid
{
	VulkanBuffer::VulkanBuffer(VulkanDevice& device) : m_device{ device } { createVertexBuffer(); }

	void VulkanBuffer::createVertexBuffer()
	{
		m_vertexCount = static_cast<uint32_t>(m_vertices.size());
		assert(m_vertexCount >= 3 && "Vertex count must be at least 3");
		VkDeviceSize bufferSize = sizeof(m_vertices[0]) * m_vertexCount;
		m_device.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			m_vertexBuffer,
			m_vertexBufferMemory);

		void* data;
		vkMapMemory(m_device.GetDevice(), m_vertexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, m_vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(m_device.GetDevice(), m_vertexBufferMemory);
	}

	void VulkanBuffer::cleanup()
	{
		vkDestroyBuffer(m_device.GetDevice(), m_vertexBuffer, nullptr);
		vkFreeMemory(m_device.GetDevice(), m_vertexBufferMemory, nullptr);
	}

	void VulkanBuffer::draw(VkCommandBuffer commandBuffer)
	{
		vkCmdDraw(commandBuffer, m_vertexCount, 1, 0, 0);
	}

	void VulkanBuffer::bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { m_vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}


} // namespace lve