#pragma once
#pragma once
#include "PipelineInfo.h"
#include <glm/glm.hpp>

namespace Revid
{
	struct DescriptorData
	{
		glm::vec3 diffuseColor;
		float shininess;

		// texture image views, samplers, etc. can be added here
	};

	struct Material
	{
		uint32_t id;
		PipelineInfo pipelineInfo;
		DescriptorData descriptorData;
	};
}
