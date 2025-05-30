#pragma once
#include <types/Containers.h>
#include "Entity.h"
#include "ComponentType.h"

namespace Revid
{
	class EntityManager
	{
	public:
		EntityManager();

		Entity CreateEntity();
		bool DeleteEntity(EntityID id);

		void SetSignature(Entity entity, Signature signature);

		Signature GetSignature(EntityID id);

	private:
		Queue<Entity> m_entities;
		Vector<size_t> m_EntityIDToIndex;
		int m_totalEntities;
	};
}
