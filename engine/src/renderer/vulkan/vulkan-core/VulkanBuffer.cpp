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

namespace Revid {
VulkanBuffer::VulkanBuffer(VulkanDevice& device) : m_device{device} { createVertexBuffer(); }

void VulkanBuffer::createVertexBuffer() {
  vertexCount = static_cast<uint32_t>(vertices.size());
  assert(vertexCount >= 3 && "Vertex count must be at least 3");
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
  m_device.createBuffer(
      bufferSize,
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
      vertexBuffer,
      vertexBufferMemory);

  void* data;
  vkMapMemory(m_device.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertices.data(), (size_t)bufferSize);
  vkUnmapMemory(m_device.device(), vertexBufferMemory);
}

void VulkanBuffer::cleanup() {
  vkDestroyBuffer(m_device.device(), vertexBuffer, nullptr);
  vkFreeMemory(m_device.device(), vertexBufferMemory, nullptr);
}

void VulkanBuffer::draw(VkCommandBuffer commandBuffer) {
  vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
}

void VulkanBuffer::bind(VkCommandBuffer commandBuffer) {
  VkBuffer buffers[] = {vertexBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

std::vector<VkVertexInputAttributeDescription> VulkanBuffer::Vertex::getAttributeDescriptions() {
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions(
      1,
      VkVertexInputAttributeDescription{});

  attributeDescriptions[0].binding = 0;
  attributeDescriptions[0].location = 0;
  attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
  attributeDescriptions[0].offset = 0;

  return attributeDescriptions;
}

}  // namespace lve