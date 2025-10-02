#pragma once

#pragma once

#include <vector>
#include "RenderGraph.h"

namespace Revid
{
	class GBufferRS
	{
	public:
		static void Record(RenderGraph& graph);
	};

	class LightingRS
	{
	public:
		static void Record(RenderGraph& graph);
	};
}
