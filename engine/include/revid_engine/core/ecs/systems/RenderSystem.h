#pragma once
#include <revid_engine/core/ecs/systems/RenderSystem.h>
#include <revid_engine/core/ecs/System.h>
#include <revid_engine/core/renderer/RenderGraph.h>

namespace Revid
{
	class RenderSystem : public System
	{
	public:
		void Update(float dt, RenderGraph& graph);
	};
}

