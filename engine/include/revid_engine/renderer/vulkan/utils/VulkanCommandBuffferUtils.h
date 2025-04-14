#pragma once
#include "revid_engine/renderer/vulkan/VulkanRenderer.h"

inline void Revid::VulkanRenderer::recordCommandBuffer(const VkCommandBuffer& commandBuffer, uint32_t imageIndex)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

	// // Transition swapchain image to COLOR_ATTACHMENT_OPTIMAL for the main render pass
	// VkImageMemoryBarrier barrier{};
	// barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	// barrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	// barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	// barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	// barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	// barrier.image = m_sceneImages[imageIndex];
	// barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	// barrier.subresourceRange.baseMipLevel = 0;
	// barrier.subresourceRange.levelCount = 1;
	// barrier.subresourceRange.baseArrayLayer = 0;
	// barrier.subresourceRange.layerCount = 1;
	// barrier.srcAccessMask = VK_ACCESS_NONE;
	// barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	//
	// vkCmdPipelineBarrier(
	// 	commandBuffer,
	// 	VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
	// 	VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
	// 	0, 0, nullptr, 0, nullptr, 1, &barrier
	// );

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_renderPass;
    renderPassInfo.framebuffer = m_sceneFrameBuffers[imageIndex];
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

    Vector<VkBuffer> vertexBuffers;
    Vector<VkDeviceSize> indexOffsets; // Offsets for each index buffer (if different offsets are required)
    vertexBuffers.reserve(MAX_MESHES_ALLOWED);
    indexOffsets.reserve(MAX_MESHES_ALLOWED);

    for (auto mesh : m_meshes)
    {
        vertexBuffers.push_back(mesh->GetVertexBuffer());
        indexOffsets.push_back(0);
    }

    for (size_t i = 0; i < m_meshes.size(); i++)
	{
        updateUniformBuffer(m_currentFrame, i);
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffers[i], &indexOffsets[i]);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_gbufferPipelineLayout, 0, 1, &m_gbufferDescriptorSets[i][m_currentFrame], 0, nullptr);
        VkBuffer indexBuffer = m_meshes[i]->GetIndexBuffer();
        int indexCount = m_meshes[i]->GetIndicesSize();
		vkCmdBindIndexBuffer(commandBuffer, indexBuffer, indexOffsets[i], VK_INDEX_TYPE_UINT16);
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

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
	{
        throw RevidRuntimeException("failed to record command buffer!");
    }
}

inline void Revid::VulkanRenderer::recordImguiCommandBuffer(const VkCommandBuffer& commandBuffer, uint32_t imageIndex)
{
    // Begin recording the command buffer
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    VkResult err = vkBeginCommandBuffer(commandBuffer, &beginInfo);
    check_vk_result(err);

    // Barrier: Transition swapchain image from PRESENT_SRC_KHR to COLOR_ATTACHMENT_OPTIMAL
    VkImageMemoryBarrier barrierToColor = {};
    barrierToColor.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrierToColor.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    barrierToColor.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    barrierToColor.srcAccessMask = 0;
    barrierToColor.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    barrierToColor.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrierToColor.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrierToColor.image = m_swapChainImages[imageIndex];
    barrierToColor.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrierToColor.subresourceRange.baseMipLevel = 0;
    barrierToColor.subresourceRange.levelCount = 1;
    barrierToColor.subresourceRange.baseArrayLayer = 0;
    barrierToColor.subresourceRange.layerCount = 1;

    vkCmdPipelineBarrier(
        commandBuffer,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,             // Source stage: nothing has used the image yet
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,   // Destination stage: about to write to the image
        0,
        0, nullptr,
        0, nullptr,
        1, &barrierToColor
    );

    // Begin ImGui render pass (using the swapchain framebuffer)
    Vector<VkClearValue> clearValues = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_imGuiRenderPass;
    renderPassInfo.framebuffer = m_swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = m_swapChainExtent;
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    // Render ImGui draw data
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

    vkCmdEndRenderPass(commandBuffer);

    // End recording the command buffer
    err = vkEndCommandBuffer(commandBuffer);
    check_vk_result(err);
}

VkCommandBuffer Revid::VulkanRenderer::beginSingleTimeCommands()
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(m_device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void Revid::VulkanRenderer::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(m_graphicsQueue);

	vkFreeCommandBuffers(m_device, m_commandPool, 1, &commandBuffer);
}


