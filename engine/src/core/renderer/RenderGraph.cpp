#include <exceptions/RevidRuntimeException.h>
#include <revid_engine/ServiceLocator.h>
#include <revid_engine/core/renderer/RenderGraph.h>

void Revid::RenderGraph::SubmitCommand(uint32_t passIndex,
	uint32_t subpass,
	const RenderCommand& renderCommand)
{
	m_passes[passIndex]->commandsPerFrame[subpass].push_back(renderCommand);
}

void Revid::RenderGraph::ExecuteAllCommands(VkCommandBuffer& cmd, uint32_t frameIndex)
{
	for (auto& node : m_passes)
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
		beginInfo.renderPass = ServiceLocator::GetRenderer()->GetRenderPass();
		beginInfo.framebuffer = ServiceLocator::GetRenderer()->GetFramebuffer(frameIndex);
		beginInfo.renderArea.offset = { 0, 0 };
		beginInfo.renderArea.extent = node->extent;
		beginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		beginInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(cmd, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);

		for (uint32_t subpass = 0; subpass < node->subpassCount; ++subpass)
		{
			if (subpass > 0)
			{
				vkCmdNextSubpass(cmd, VK_SUBPASS_CONTENTS_INLINE);
			}

			const auto& renderCommands = node->commandsPerFrame[subpass];
			for (const auto& rc : renderCommands)
			{
				rc(cmd, frameIndex);
			}
		}

		vkCmdEndRenderPass(cmd);
	}

	for (auto& node : m_passes)
	{
		for (uint32_t subpass = 0; subpass < node->subpassCount; ++subpass)
		{
			node->commandsPerFrame[subpass].clear();
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

void Revid::RenderGraph::AddPassFromTemplate(const RenderPassTemplate& tmpl,
	VkDevice device,
	const Vector<VkImageView>& swapchainViews)
{
	auto node = MakePtr<RenderPassNode>();
	node->name = tmpl.name;
	node->extent = tmpl.extent;
	node->subpassCount = static_cast<uint32_t>(tmpl.subpasses.size());

	Vector<VkAttachmentDescription> attachmentDescs;
	std::unordered_map<String, uint32_t> attachmentIndices;
	for (size_t i = 0; i < tmpl.attachments.size(); ++i)
	{
		const auto& att = tmpl.attachments[i];
		attachmentDescs.push_back(MakeAttachmentDesc(att));
		attachmentIndices[att.name] = static_cast<uint32_t>(i);
		node->attachments.push_back(att);
	}

	Vector<VkSubpassDescription> subpasses;
	Vector<Vector<VkAttachmentReference>> colorRefsPerSubpass;
	Vector<Vector<VkAttachmentReference>> inputRefsPerSubpass;
	Vector<VkAttachmentReference> depthRefs;

	for (const auto& sub : tmpl.subpasses)
	{
		VkSubpassDescription subDesc{};
		subDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		auto& colorRefs = colorRefsPerSubpass.emplace_back();
		for (const auto& colorRef : sub.colorAttachments)
		{
			colorRefs.push_back({
				attachmentIndices[colorRef.name],
				VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
			});
		}

		auto& inputRefs = inputRefsPerSubpass.emplace_back();
		for (const auto& inputRef : sub.inputAttachments)
		{
			inputRefs.push_back({
				attachmentIndices[inputRef.name],
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			});
		}

		subDesc.colorAttachmentCount = static_cast<uint32_t>(colorRefs.size());
		subDesc.pColorAttachments = colorRefs.data();
		subDesc.inputAttachmentCount = static_cast<uint32_t>(inputRefs.size());
		subDesc.pInputAttachments = inputRefs.data();


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

	// Subpass dependencies
	Vector<VkSubpassDependency> dependencies;
	for (uint32_t i = 0; i < tmpl.subpasses.size() - 1; ++i)
	{
		const auto& sub = tmpl.subpasses[i];
		VkSubpassDependency dep{};
		dep.srcSubpass = i;
		dep.dstSubpass = i + 1;
		dep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dep.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dep.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dep.dstAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		dep.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		if (sub.readFromPrevious && i > 0)
		{
			dep.srcSubpass = i - 1;
			dep.srcStageMask |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			dep.srcAccessMask |= VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
		}

		dependencies.push_back(dep);
	}
	// Last subpass to external
	VkSubpassDependency externalDep{};
	externalDep.srcSubpass = tmpl.subpasses.size() - 1;
	externalDep.dstSubpass = VK_SUBPASS_EXTERNAL;
	externalDep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	externalDep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	externalDep.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	externalDep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	externalDep.dependencyFlags = 0;
	dependencies.push_back(externalDep);


	VkRenderPassCreateInfo passInfo{};
	passInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	passInfo.attachmentCount = static_cast<uint32_t>(attachmentDescs.size());
	passInfo.pAttachments = attachmentDescs.data();
	passInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
	passInfo.pSubpasses = subpasses.data();
	passInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
	passInfo.pDependencies = dependencies.data();

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
				continue;
			}

			VkImageUsageFlags usage;
			VkImageAspectFlags aspectFlags;
			if (att.isDepth)
			{
				usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
			}
			else
			{
				usage = (VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
				aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
			}
			views.push_back(ServiceLocator::GetRenderer()->CreateAttachment(device, att.format, tmpl.extent, usage, aspectFlags).view); // You implement this
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

	m_passes.push_back(std::move(node));
}
