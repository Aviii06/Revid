#pragma once

#include <vulkan/vulkan.h>
#include "VulkanDevice.h"

// std
#include <string>
#include <vector>

namespace Revid {

struct PipelineConfigInfo {
  VkViewport viewport;
  VkRect2D scissor;
  VkPipelineViewportStateCreateInfo viewportInfo;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  VkPipelineLayout pipelineLayout = nullptr;
  VkRenderPass renderPass = nullptr;
  uint32_t subpass = 0;
};

class VulkanPipeline {
 public:
  VulkanPipeline(
      VulkanDevice& device,
      const std::string& vertFilepath,
      const std::string& fragFilepath,
      const PipelineConfigInfo& configInfo);
  ~VulkanPipeline();

  VulkanPipeline(const VulkanPipeline&) = delete;
  void operator=(const VulkanPipeline&) = delete;

  void bind(VkCommandBuffer commandBuffer);

  static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

 private:
  void createGraphicsPipeline(
      const std::string& vertFilepath,
      const std::string& fragFilepath,
      const PipelineConfigInfo& configInfo);
  void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

  VulkanDevice& m_device;
  VkPipeline m_graphicsPipeline;
  VkShaderModule m_vertShaderModule;
  VkShaderModule m_fragShaderModule;
};
}  // namespace lve