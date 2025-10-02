#include "revid_engine/core/ecs/systems/RenderSystem.h"

#include "revid_engine/core/ecs/components/model/ModelComponent.h"
#include <revid_engine/core/ecs/ECS.h>
#include <revid_engine/core/renderer/RenderStages.h>

namespace Revid
{
	void RenderSystem::Update(float dt, RenderGraph& graph)
	{
		GBufferRS::Record(graph);
		LightingRS::Record(graph);
	}
}

