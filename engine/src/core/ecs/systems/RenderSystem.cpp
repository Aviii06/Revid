#include "revid_engine/core/ecs/systems/RenderSystem.h"

#include "revid_engine/core/ecs/components/model/ModelComponent.h"
#include <revid_engine/core/ecs/ECS.h>

namespace Revid
{
	void RenderSystem::Update(float dt)
	{
		for (auto const& entity : m_entities)
		{
			auto& mc = ServiceLocator::GetECSCoordinator()->GetComponent<ModelComponent>(entity);
			for (auto& x : mc.m_meshes)
			{
			}
		}
	}
}

