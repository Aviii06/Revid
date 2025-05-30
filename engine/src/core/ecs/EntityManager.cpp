#include "revid_engine/core/ecs/EntityManager.h"

namespace Revid
{

	EntityManager::EntityManager()
	{
		m_entities.reserve(MAX_ENTITIES);
		m_EntityIDToIndex.reserve(MAX_ENTITIES);
		m_totalEntities = 0;

		for (int i = 0; i < MAX_ENTITIES; i++)
		{
			m_EntityIDToIndex.push_back(-1);
		}
	}

	Entity EntityManager::CreateEntity()
	{
		m_entities.push_back({m_totalEntities, 0});
		m_EntityIDToIndex[m_totalEntities] = m_totalEntities++;

		return {m_totalEntities - 1, 0};
	}

	inline bool EntityManager::DeleteEntity(EntityID id)
	{
		if (m_EntityIDToIndex[id] == -1)
		{
			return false;
		}

		m_entities[m_EntityIDToIndex[id]] = m_entities[m_totalEntities - 1];
		m_EntityIDToIndex[m_totalEntities - 1] = m_EntityIDToIndex[id];
		m_EntityIDToIndex[id] = -1;

		m_entities.pop_back();
		return true;
	}

	inline Signature EntityManager::GetComponents(EntityID id)
	{
		int index = m_EntityIDToIndex[id];
		if (index == -1)
		{
			return 0;
		}
		Entity entity = m_entities[index];
		return entity.GetSignature();
	}
}

