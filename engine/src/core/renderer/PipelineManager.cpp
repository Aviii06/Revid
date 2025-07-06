#include "revid_engine/core/renderer/PipelineManager.h"
#include <revid_engine/core/renderer/Shader.h>
using namespace Revid;

// void PipelineManager::Init(VkDevice device)
// {
//     m_device = device;
// }
//
// const PipelineBundle& PipelineManager::GetOrCreatePipeline(const PipelineInfo& info)
// {
//     auto it = m_pipelineCache.find(info);
//     if (it != m_pipelineCache.end()) return it->second;
//
//     PipelineBundle bundle = CreatePipeline(info);
//     auto [iter, success] = m_pipelineCache.emplace(info, bundle);
//     return iter->second;
// }
//
// PipelineBundle PipelineManager::CreatePipeline(const PipelineInfo& info)
// {
// 	PipelineBundle bundle{};
// 	Shader vertShader(info.vertexShaderPath);
// 	Shader fragShader(info.vertexShaderPath);
//
// 	VkShaderModule vertShaderModule = vertShader.GetShaderModule();
// 	VkShaderModule fragShaderModule = fragShader.GetShaderModule();
//
// 	std::vector<VkPipelineShaderStageCreateInfo> shaderStages(2);
//
// 	shaderStages[0] = {
// 		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
// 		.stage = VK_SHADER_STAGE_VERTEX_BIT,
// 		.module = vertShaderModule,
// 		.pName = "main"
// 	};
//
// 	shaderStages[1] = {
// 		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
// 		.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
// 		.module = fragShaderModule,
// 		.pName = "main"
// 	};
//
// 	for (const auto& binding : info.descriptorBindings)
// 	{
// 		VkDescriptorSetLayoutCreateInfo layoutInfo{};
// 		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
// 		layoutInfo.bindingCount = 1;
// 		layoutInfo.pBindings = &binding;
//
// 		VkDescriptorSetLayout layout;
// 		if (vkCreateDescriptorSetLayout(m_device, &layoutInfo, nullptr, &layout) != VK_SUCCESS)
// 			throw std::runtime_error("Failed to create descriptor set layout");
//
// 		bundle.descriptorSetLayouts.push_back(layout);
// 	}
//
// 	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
// 	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
// 	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(bundle.descriptorSetLayouts.size());
// 	pipelineLayoutInfo.pSetLayouts = bundle.descriptorSetLayouts.data();
// 	pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(info.pushConstants.size());
// 	pipelineLayoutInfo.pPushConstantRanges = info.pushConstants.data();
//
// 	if (vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &bundle.layout) != VK_SUCCESS)
// 		throw std::runtime_error("Failed to create pipeline layout");
//
// 	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
// 	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
//
// 	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
// 	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
// 	inputAssembly.topology = info.topology;
// 	inputAssembly.primitiveRestartEnable = VK_FALSE;
//
// 	VkViewport viewport{};
// 	VkRect2D scissor{};
// 	VkPipelineViewportStateCreateInfo viewportState{};
// 	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
// 	viewportState.viewportCount = 1;
// 	viewportState.pViewports = &viewport;
// 	viewportState.scissorCount = 1;
// 	viewportState.pScissors = &scissor;
//
// 	VkPipelineRasterizationStateCreateInfo rasterizer{};
// 	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
// 	rasterizer.depthClampEnable = VK_FALSE;
// 	rasterizer.rasterizerDiscardEnable = VK_FALSE;
// 	rasterizer.polygonMode = info.polygonMode;
// 	rasterizer.lineWidth = 1.0f;
// 	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
// 	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
// 	rasterizer.depthBiasEnable = VK_FALSE;
//
// 	VkPipelineMultisampleStateCreateInfo multisampling{};
// 	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
// 	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
//
// 	VkPipelineDepthStencilStateCreateInfo depthStencil{};
// 	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
// 	depthStencil.depthTestEnable = VK_TRUE;
// 	depthStencil.depthWriteEnable = VK_TRUE;
// 	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
//
// 	Vector<VkPipelineColorBlendAttachmentState> colorBlendAttachment;
// 	colorBlendAttachment.reserve(info.numberOfColorAttachments);
// 	for (size_t i = 0; i < info.numberOfColorAttachments; ++i)
// 	{
// 		colorBlendAttachment.push_back({
// 			.blendEnable = VK_FALSE, // No blending
// 			.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
// 			.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
// 			.colorBlendOp = VK_BLEND_OP_ADD, // Default,
// 			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
// 			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
// 			.alphaBlendOp = VK_BLEND_OP_ADD, // Default,
// 			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
// 			VK_COLOR_COMPONENT_G_BIT |
// 			VK_COLOR_COMPONENT_B_BIT |
// 			VK_COLOR_COMPONENT_A_BIT
// 		});
// 	}
//
// 	VkPipelineColorBlendStateCreateInfo colorBlending{};
// 	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
// 	colorBlending.logicOpEnable = VK_FALSE;
// 	colorBlending.attachmentCount = info.numberOfColorAttachments;
// 	colorBlending.pAttachments = colorBlendAttachment.data();
//
// 	std::vector<VkDynamicState> dynamicStates = {
// 		VK_DYNAMIC_STATE_VIEWPORT,
// 		VK_DYNAMIC_STATE_SCISSOR
// 	};
//
// 	VkPipelineDynamicStateCreateInfo dynamicState{};
// 	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
// 	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
// 	dynamicState.pDynamicStates = dynamicStates.data();
//
// 	VkGraphicsPipelineCreateInfo pipelineInfo{};
// 	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
// 	pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
// 	pipelineInfo.pStages = shaderStages.data();
// 	pipelineInfo.pVertexInputState = &vertexInputInfo;
// 	pipelineInfo.pInputAssemblyState = &inputAssembly;
// 	pipelineInfo.pViewportState = &viewportState;
// 	pipelineInfo.pRasterizationState = &rasterizer;
// 	pipelineInfo.pMultisampleState = &multisampling;
// 	pipelineInfo.pDepthStencilState = &depthStencil;
// 	pipelineInfo.pColorBlendState = &colorBlending;
// 	pipelineInfo.pDynamicState = &dynamicState;
// 	pipelineInfo.layout = bundle.layout;
// 	pipelineInfo.renderPass = info.renderPass;
// 	pipelineInfo.subpass = info.subpass;
// 	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
//
// 	if (vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &bundle.pipeline) != VK_SUCCESS)
// 		throw std::runtime_error("Failed to create graphics pipeline");
//
// 	vkDestroyShaderModule(m_device, vertShaderModule, nullptr);
// 	vkDestroyShaderModule(m_device, fragShaderModule, nullptr);
//
// 	return bundle;
// }
//
// void PipelineManager::Cleanup()
// {
// 	for (auto& [_, bundle] : m_pipelineCache)
// 	{
// 		for (auto layout : bundle.descriptorSetLayouts)
// 		{
// 			vkDestroyDescriptorSetLayout(m_device, layout, nullptr);
// 		}
// 		vkDestroyPipeline(m_device, bundle.pipeline, nullptr);
// 		vkDestroyPipelineLayout(m_device, bundle.layout, nullptr);
// 	}
// }
