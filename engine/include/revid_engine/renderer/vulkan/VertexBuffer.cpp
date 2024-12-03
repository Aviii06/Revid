#include "VertexBuffer.h"
#include <revid_engine/ServiceLocater.h>

Revid::VertexBuffer::VertexBuffer()
	: m_bf(Buffer())
{
	m_device = ServiceLocator::GetRenderer()->GetDevice();
	createVertexBuffer();
}

Revid::VertexBuffer::VertexBuffer(const Vector<SimpleVertex>& verts)
	: m_vertices(std::move(verts)), m_bf(Buffer())
{
	m_device = ServiceLocator::GetRenderer()->GetDevice();
	createVertexBuffer();
}

void Revid::VertexBuffer::SetVertices(const Vector<SimpleVertex>& verts)
{
	m_vertices = verts;
	createVertexBuffer();
}

void Revid::VertexBuffer::createVertexBuffer()
{
	VkDeviceSize bufferSize = sizeof(m_vertices[0]) * m_vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	m_bf.CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(m_device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, m_vertices.data(), (size_t) bufferSize);
	vkUnmapMemory(m_device, stagingBufferMemory);

	VkBuffer buf;
	m_bf.CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buf, m_vertexBufferMemory);

	m_bf.CopyBuffer(stagingBuffer, buf, bufferSize);
	m_vertexBuffers.emplace_back(std::move(buf));

	vkDestroyBuffer(m_device, stagingBuffer, nullptr);
	vkFreeMemory(m_device, stagingBufferMemory, nullptr);
}


