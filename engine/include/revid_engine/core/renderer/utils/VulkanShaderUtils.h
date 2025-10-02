#pragma once
#include <revid_engine/core/renderer/Renderer.h>
#include <exceptions/RevidRuntimeException.h>

VkShaderModule Revid::VulkanRenderer::createShaderModule(const std::vector<char>& code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(m_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
        throw RevidRuntimeException("failed to create shader module!");
    }

    return shaderModule;
}
