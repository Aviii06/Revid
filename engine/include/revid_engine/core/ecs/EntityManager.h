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
		bool DeleteEntity(Entity id);
		void SetSignature(Entity entity, Signature signature);
		Signature GetSignature(Entity entity) const;

	private:
		Queue<Entity> m_availableEntities;
		Array<Signature, MAX_ENTITIES> m_signatures;
		uint32_t m_livingEntities;
	};
}
