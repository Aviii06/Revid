#pragma once
#include "renderer/vulkan/VulkanRenderer.h"

inline void Revid::VulkanRenderer::recordCommandBuffer(const VkCommandBuffer& commandBuffer, uint32_t imageIndex)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_renderPass;
    renderPassInfo.framebuffer = m_swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = m_swapChainExtent;

    VkClearValue clearValues[4] = {
        { {0.0f, 0.0f, 0.0f, 1.0f} }, // Position
        { {0.0f, 0.0f, 0.0f, 1.0f} }, // Color
        { {0.0f, 0.0f, 0.0f, 1.0f} }, // Normal
        { {0.0f, 0.0f, 0.0f, 1.0f} }  // Swapchain
    };
    renderPassInfo.clearValueCount = 4;
    renderPassInfo.pClearValues = clearValues;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    // Geometry pass
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_gbufferPipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) m_swapChainExtent.width;
    viewport.height = (float) m_swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = m_swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    Vector<VkBuffer> vertexBuffers;
    Vector<VkBuffer> indexBuffers; // Example index buffers
    Vector<VkDeviceSize> indexOffsets; // Offsets for each index buffer (if different offsets are required)
    vertexBuffers.reserve(MAX_MESHES_ALLOWED);
    indexBuffers.reserve(MAX_MESHES_ALLOWED);
    indexOffsets.reserve(MAX_MESHES_ALLOWED);

    for (auto mesh : m_meshes)
    {
        vertexBuffers.push_back(mesh->GetVertexBuffer());
        indexBuffers.push_back(mesh->GetIndexBuffer());
        indexOffsets.push_back(0);
    }

    vkCmdBindVertexBuffers(commandBuffer, 0, vertexBuffers.size(), vertexBuffers.data(), indexOffsets.data());
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_gbufferPipelineLayout, 0, 1, &m_gbufferDescriptorSets[m_currentFrame], 0, nullptr);
    for (size_t i = 0; i < m_meshes.size(); i++)
	{
		vkCmdBindIndexBuffer(commandBuffer, indexBuffers[i], indexOffsets[i], VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_indices.size()), m_meshes[i]->GetInstanceCount(), 0, 0, 0);
	}

    // vkCmdDraw(commandBuffer, static_cast<uint32_t>(m_vertices.size()), 1, 0, 0);
    // vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_indices.size()), 10000, 0, 0, 0);

    // vkCmdDraw(commandBuffer, 3, 1, 0, 0);

    vkCmdNextSubpass(commandBuffer, VK_SUBPASS_CONTENTS_INLINE);

    // Lighting subpass
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_lightingPipeline);

    VkBuffer vertexBuffers2[] = {m_lightingVertexBuffer};
    VkDeviceSize offsets2[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers2, offsets2);
    vkCmdBindIndexBuffer(commandBuffer, m_lightingIndexBuffer, 0, VK_INDEX_TYPE_UINT16);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_lightingPipelineLayout, 0, 1, &m_lightingDescriptorSets[m_currentFrame], 0, nullptr);
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_indices2.size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
	{
        throw RevidRuntimeException("failed to record command buffer!");
    }
}

