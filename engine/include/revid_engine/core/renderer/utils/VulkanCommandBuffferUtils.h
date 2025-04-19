#pragma once
#include <revid_engine/core/renderer/VulkanRenderer.h>

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

    Vector<VkClearValue> clearValues = {
        { {0.0f, 0.0f, 0.0f, 1.0f} }, // Position
        { {0.0f, 0.0f, 0.0f, 1.0f} }, // Color
        { {0.0f, 0.0f, 0.0f, 1.0f} }, // Normal
        {}, // Depth
        { {0.0f, 0.0f, 0.0f, 1.0f} }  // Swapchain
    };

    clearValues[3].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

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

    for (size_t i = 0; i < m_meshes.size(); i++)
	{
        updateUniformBuffer(m_currentFrame, i);
        VkDeviceSize indexOffset = 0;
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, m_meshes[i]->GetVertexBuffer().get(), &indexOffset);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_gbufferPipelineLayout, 0, 1, &m_gbufferDescriptorSets[i][m_currentFrame], 0, nullptr);
        VkBuffer indexBuffer = m_meshes[i]->GetIndexBuffer();
        int indexCount = m_meshes[i]->GetIndicesSize();
		vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indexCount), m_meshes[i]->GetInstanceCount(), 0, 0, 0);
	}

    vkCmdNextSubpass(commandBuffer, VK_SUBPASS_CONTENTS_INLINE);

    // Lighting subpass
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_lightingPipeline);

    VkBuffer vertexBuffers2[] = {m_lightingVertexBuffer};
    VkDeviceSize offsets2[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers2, offsets2);
    vkCmdBindIndexBuffer(commandBuffer, m_lightingIndexBuffer, 0, VK_INDEX_TYPE_UINT16);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_lightingPipelineLayout, 0, 1, &m_lightingDescriptorSets[m_currentFrame], 0, nullptr);
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_indices2.size()), 1, 0, 0, 0);

    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
	{
        throw RevidRuntimeException("failed to record command buffer!");
    }
}

