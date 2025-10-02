#pragma once
#include <types/Containers.h>
#include <vulkan/vulkan.h>

namespace Revid
{
    class Shader
    {
    public:
        VkShaderModule GetShaderModule() const { return m_shaderModule; }
        Shader(const String& shaderPath);
    private:
        String m_shaderPath;
        VkShaderModule m_shaderModule;
    };
}
