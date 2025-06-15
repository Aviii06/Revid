#include <cassert>
#include "revid_engine/core/ecs/EntityManager.h"

namespace Revid
{

	EntityManager::EntityManager()
	{
		m_livingEntities = 0;

		for (int entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			// TODO: Reserve this?
			m_availableEntities.push(entity);
		}
	}

	Entity EntityManager::CreateEntity()
	{
		assert(m_livingEntities < MAX_ENTITIES && "Too many entities in existence.");

		Entity entity = m_availableEntities.front();
		m_availableEntities.pop();
		++m_livingEntities;

		return entity;
	}

	inline bool EntityManager::DeleteEntity(Entity entity)
	{
		m_signatures[entity].reset();

		m_availableEntities.push(entity);
		--m_livingEntities;

		return true;
	}

	void EntityManager::SetSignature(Entity entity, Signature signature)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		m_signatures[entity] = signature;
	}

	Signature EntityManager::GetSignature(Entity entity) const
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		return m_signatures[entity];
	}
}

