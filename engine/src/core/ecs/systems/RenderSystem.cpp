#include "RenderSystem.h"

#include "revid_engine/core/ecs/components/model/ModelComponent.h"
#include <revid_engine/core/ecs/ECS.h>

namespace Revid
{
	void RenderSystem::Update(Camera* camera)
	{
		for (auto const& entity : m_entities)
		{
			auto& mc = ECS::g_component_manager->GetComponent<ModelComponent>(entity);
			for (auto& x : mc.m_meshes)
			{
				x->Draw(camera);
			}
		}
	}
}

