#include <exceptions/RevidRuntimeException.h>
#include <revid_engine/ServiceLocator.h>
#include <revid_engine/core/renderer/RenderGraph.h>

void Revid::RenderGraph::SubmitCommand(const std::string& passName,
	uint32_t subpass,
	uint32_t frameIndex,
	const RenderCommand& cmd)
{
	m_passes[passName]->commandsPerFrame[frameIndex][subpass].push_back(cmd);
}

void Revid::RenderGraph::ExecuteAllCommands(VkCommandBuffer cmd, uint32_t frameIndex)
{
	for (auto& [name, node] : m_passes)
	{
		std::vector<VkClearValue> clearValues;
		for (const auto& att : node->attachments)
		{
			VkClearValue clear{};
			if (att.isDepth)
				clear.depthStencil = { 1.0f, 0 };
			else
				clear.color = { 0.0f, 0.0f, 0.0f, 1.0f };
			clearValues.push_back(clear);
		}

		VkRenderPassBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		beginInfo.renderPass = node->renderPass;
		beginInfo.framebuffer = node->framebuffers[frameIndex];
		beginInfo.renderArea.offset = { 0, 0 };
		beginInfo.renderArea.extent = node->extent;
		beginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		beginInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(cmd, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);

		for (uint32_t subpass = 0; subpass < node->subpassCount; ++subpass)
		{
			if (subpass > 0)
				vkCmdNextSubpass(cmd, VK_SUBPASS_CONTENTS_INLINE);

			const auto& commands = node->commandsPerFrame[frameIndex][subpass];
			for (const auto& rc : commands)
			{
				vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, rc.pipeline);

				VkDeviceSize offset = 0;
				vkCmdBindVertexBuffers(cmd, 0, 1, &rc.vertexBuffer, &offset);
				vkCmdBindIndexBuffer(cmd, rc.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
				vkCmdBindDescriptorSets(
					cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, rc.layout,
					0, 1, &rc.descriptorSet, 0, nullptr
					);

				vkCmdDrawIndexed(cmd, rc.indexCount, rc.instanceCount, 0, 0, 0);
			}
		}

		vkCmdEndRenderPass(cmd);
	}

	for (auto& [_, node] : m_passes)
	{
		for (uint32_t subpass = 0; subpass < node->subpassCount; ++subpass)
		{
			node->commandsPerFrame[frameIndex][subpass].clear();
		}
	}
}

VkAttachmentDescription MakeAttachmentDesc(const Revid::AttachmentTemplate& att)
{
	VkAttachmentDescription desc{};
	desc.format = att.format;
	desc.samples = VK_SAMPLE_COUNT_1_BIT;
	desc.loadOp = att.clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
	desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	desc.initialLayout = att.isDepth
		? VK_IMAGE_LAYOUT_UNDEFINED
		: VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	desc.finalLayout = att.isSwapchain
		? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
		: (att.isDepth
			? VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
			: VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

	return desc;
}

void Revid::RenderGraph::CreatePassFromTemplate(const RenderPassTemplate& tmpl,
	VkDevice device,
	const std::vector<VkImageView>& swapchainViews)
{
	auto node = MakePtr<RenderPassNode>();
	node->name = tmpl.name;
	node->extent = tmpl.extent;
	node->subpassCount = static_cast<uint32_t>(tmpl.subpasses.size());

	std::vector<VkAttachmentDescription> attachmentDescs;
	std::unordered_map<std::string, uint32_t> attachmentIndices;
	for (size_t i = 0; i < tmpl.attachments.size(); ++i)
	{
		const auto& att = tmpl.attachments[i];
		attachmentDescs.push_back(MakeAttachmentDesc(att));
		attachmentIndices[att.name] = static_cast<uint32_t>(i);
		node->attachments.push_back(att);
	}

	std::vector<VkSubpassDescription> subpasses;
	std::vector<std::vector<VkAttachmentReference>> colorRefsPerSubpass;
	std::vector<VkAttachmentReference> depthRefs;

	for (const auto& sub : tmpl.subpasses)
	{
		VkSubpassDescription subDesc{};
		subDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		auto& colorRefs = colorRefsPerSubpass.emplace_back();
		for (const auto& colorName : sub.colorAttachments)
		{
			colorRefs.push_back({
				attachmentIndices[colorName],
				VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
			});
		}

		subDesc.colorAttachmentCount = static_cast<uint32_t>(colorRefs.size());
		subDesc.pColorAttachments = colorRefs.data();

		if (sub.depthAttachment.has_value())
		{
			depthRefs.push_back({
				attachmentIndices[sub.depthAttachment.value()],
				VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
			});
			subDesc.pDepthStencilAttachment = &depthRefs.back();
		}

		subpasses.push_back(subDesc);
	}

	VkRenderPassCreateInfo passInfo{};
	passInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	passInfo.attachmentCount = static_cast<uint32_t>(attachmentDescs.size());
	passInfo.pAttachments = attachmentDescs.data();
	passInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
	passInfo.pSubpasses = subpasses.data();

	if (vkCreateRenderPass(device, &passInfo, nullptr, &node->renderPass) != VK_SUCCESS)
	{
		throw RevidRuntimeException("Failed to create render pass: " + tmpl.name);
	}

	int framesInFlight = swapchainViews.size();

	for (uint32_t frame = 0; frame < framesInFlight; ++frame)
	{
		std::vector<VkImageView> views;
		for (const auto& att : tmpl.attachments)
		{
			if (att.isSwapchain)
			{
				views.push_back(swapchainViews[frame]);
			}
			else
			{
				VkImageUsageFlags usage = att.isDepth
					? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
					: (VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);

				VkImageAspectFlags aspectMask = att.isDepth
					? VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT
					: VK_IMAGE_ASPECT_COLOR_BIT;

				// views.push_back(ServiceLocator::GetRenderer()->CreateAttachment(device, att.format, tmpl.extent, usage, aspectMask).view); // You implement this
			}
		}

		VkFramebufferCreateInfo fbInfo{};
		fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fbInfo.renderPass = node->renderPass;
		fbInfo.attachmentCount = static_cast<uint32_t>(views.size());
		fbInfo.pAttachments = views.data();
		fbInfo.width = tmpl.extent.width;
		fbInfo.height = tmpl.extent.height;
		fbInfo.layers = 1;

		VkFramebuffer framebuffer;
		if (vkCreateFramebuffer(device, &fbInfo, nullptr, &framebuffer) != VK_SUCCESS)
		{
			throw RevidRuntimeException("Failed to create framebuffer for pass: " + tmpl.name);
		}
		node->framebuffers.push_back(framebuffer);
	}

	node->Initialize(framesInFlight);
	m_passes[tmpl.name] = std::move(node);
}
