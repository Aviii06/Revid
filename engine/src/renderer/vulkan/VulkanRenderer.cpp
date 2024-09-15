#include "VulkanRenderer.h"

#include <exceptions/RevidRuntimeException.h>

void Revid::VulkanRenderer::Init(const RendererSettings& rendererSettings)
{
	m_swapchain = MakePtr<VulkanSwapchain>(m_device, rendererSettings.MAX_FRAMES_IN_FLIGHT);
	createPipelineLayout();
	createPipeline();
	createCommandBuffers();
}


void Revid::VulkanRenderer::Shutdown()
{
	vkDestroyPipelineLayout(m_device.GetDevice(), m_pipelineLayout, nullptr);
	vkFreeCommandBuffers(
		m_device.GetDevice(),
		m_device.GetCommandPool(),
		static_cast<uint32_t>(m_commandBuffers.size()),
		m_commandBuffers.data());
	m_commandBuffers.clear();
}

void Revid::VulkanRenderer::Render()
{
	uint32_t imageIndex;
	auto res = m_swapchain->acquireNextImage(imageIndex);
	if (res == VK_ERROR_OUT_OF_DATE_KHR ||  res == VK_SUBOPTIMAL_KHR || m_framebufferResized)
	{
		m_framebufferResized = false;
		m_swapchain->recreateSwapChain();
	}
	else if (res != VK_SUCCESS)
	{
		throw RevidRuntimeException("failed to acquire swap chain image!");
	}

	res = m_swapchain->submitCommandBuffers(m_commandBuffers, imageIndex);
	if (res != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present swap chain image!");
	}
}

void Revid::VulkanRenderer::createPipelineLayout()
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;
	if (vkCreatePipelineLayout(m_device.GetDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) !=
		VK_SUCCESS)
	{
		throw RevidRuntimeException("failed to create pipeline layout!");
	}
}

void Revid::VulkanRenderer::createPipeline()
{
	auto pipelineConfig = VulkanPipeline::defaultPipelineConfigInfo(m_swapchain->width(), m_swapchain->height());
	pipelineConfig.renderPass = m_swapchain->getRenderPass();
	pipelineConfig.pipelineLayout = m_pipelineLayout;
	m_simplePipeline = MakePtr<VulkanPipeline>(
		m_device,
		"./assets/shaders/BasicShader.vert.spv",
		"./assets/shaders/BasicShader.frag.spv",
		pipelineConfig);
}

void Revid::VulkanRenderer::createCommandBuffers()
{
	m_commandBuffers.resize(m_swapchain->imageCount());
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_device.GetCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

	if (vkAllocateCommandBuffers(m_device.GetDevice(), &allocInfo, m_commandBuffers.data()) !=
		VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
		}

	for (size_t i = 0; i < m_commandBuffers.size(); i++) {
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0;                   // Optional
		beginInfo.pInheritanceInfo = nullptr;  // Optional

		if (vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}
		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_swapchain->getRenderPass();
		renderPassInfo.framebuffer = m_swapchain->getFrameBuffer(i);

		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = m_swapchain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
		clearValues[1].depthStencil = {1.0f, 0};
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		m_simplePipeline->bind(m_commandBuffers[i]);

		vkCmdDraw(m_commandBuffers[i], 3, 1, 0, 0);

		vkCmdEndRenderPass(m_commandBuffers[i]);
		if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}
