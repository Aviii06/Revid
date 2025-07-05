#include "revid_engine/core/renderer/Shader.h"
#include <utils/FileHandler.h>
#include <revid_engine/ServiceLocator.h>

using namespace Revid;

Shader::Shader(String& shaderPath)
	: m_shaderPath(shaderPath)
{
	std::vector<char> shaderCode = RevidUtils::ReadFile(shaderPath);

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = shaderCode.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

	if (vkCreateShaderModule(ServiceLocator::GetRenderer()->GetDeivce(), &createInfo, nullptr, &m_shaderModule) != VK_SUCCESS)
	{
		throw RevidRuntimeException("Failed to create shader module from path: " + shaderPath);
	}
}
