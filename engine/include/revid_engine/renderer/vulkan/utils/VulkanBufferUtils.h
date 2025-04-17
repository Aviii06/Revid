#pragma once
#include <revid_engine/renderer/vulkan/Vertex.h>

void Revid::VulkanRenderer::createBuffer(VmaAllocator allocator,
                                         VkDeviceSize size,
                                         VkBufferUsageFlags usage,
                                         VmaMemoryUsage memoryUsage,
                                         VkBuffer& buffer,
                                         VmaAllocation& allocation,
                                         void** mappedPtr,
                                         VkBufferCreateFlags createFlags)
{
	VkBufferCreateInfo bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferInfo.flags = createFlags;

	VmaAllocationCreateInfo allocCreateInfo = {};
	allocCreateInfo.usage = memoryUsage;

	VkResult result;
	if (mappedPtr)
	{
		allocCreateInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT |
								VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

		VmaAllocationInfo allocInfo;
		result = vmaCreateBuffer(allocator, &bufferInfo, &allocCreateInfo, &buffer, &allocation, &allocInfo);

		if (result != VK_SUCCESS)
		{
			std::cerr << "vmaCreateBuffer failed with error code: " << result << std::endl;
			return;
		}

		*mappedPtr = allocInfo.pMappedData;
	}
	else
	{
		result = vmaCreateBuffer(allocator, &bufferInfo, &allocCreateInfo, &buffer, &allocation, nullptr);

		if (result != VK_SUCCESS)
		{
			std::cerr << "vmaCreateBuffer failed with error code: " << result << std::endl;
			return;
		}
	}

	// Additional check to ensure allocation is valid
	if (allocation == VK_NULL_HANDLE)
	{
		std::cerr << "Memory allocation failed: allocation handle is null!" << std::endl;
		return;
	}
}

uint32_t Revid::VulkanRenderer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw RevidRuntimeException("failed to find suitable memory type!");
}

void Revid::VulkanRenderer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	VkCommandBuffer commandBuffer = beginSingleTimeCommands();

	VkBufferCopy copyRegion{};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	endSingleTimeCommands(commandBuffer);
}

template <typename T>
void Revid::VulkanRenderer::createGenericBuffer(
	const std::vector<T>& data,
	VkBufferUsageFlags usageFlags,
	VmaMemoryUsage memoryUsage,
	VkBuffer& buffer,
	VmaAllocation& allocation)
{
	VkDeviceSize bufferSize = sizeof(T) * data.size();

	// === Create Staging Buffer ===
	VkBuffer stagingBuffer;
	VmaAllocation stagingAlloc;
	void* stagingData = nullptr;

	createBuffer(m_allocator,
				 bufferSize,
				 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				 VMA_MEMORY_USAGE_AUTO,
				 stagingBuffer,
				 stagingAlloc,
				 &stagingData);

	// Copy data into the staging buffer
	memcpy(stagingData, data.data(), bufferSize);

	// === Create GPU-local Buffer ===
	createBuffer(m_allocator,
				 bufferSize,
				 usageFlags,
				 memoryUsage,
				 buffer,
				 allocation);

	// === Copy Data from Staging to GPU-local Buffer ===
	copyBuffer(stagingBuffer, buffer, bufferSize);

	// === Cleanup Staging Buffer ===
	vmaDestroyBuffer(m_allocator, stagingBuffer, stagingAlloc);
}

template void Revid::VulkanRenderer::createGenericBuffer<uint16_t>(const std::vector<uint16_t>&, VkBufferUsageFlags, VmaMemoryUsage, VkBuffer&, VmaAllocation&);
template void Revid::VulkanRenderer::createGenericBuffer<float>(const std::vector<float>&, VkBufferUsageFlags, VmaMemoryUsage, VkBuffer&, VmaAllocation&);
template void Revid::VulkanRenderer::createGenericBuffer<Revid::SimpleVertex>(const std::vector<SimpleVertex>&, VkBufferUsageFlags, VmaMemoryUsage, VkBuffer&, VmaAllocation&);
