#include "revid_engine/core/ecs/SystemManager.h"

void Revid::SystemManager::EntitySignatureChanged(Entity entity, Signature entitySignature)
{
	for (auto const& x : m_systems)
	{
		auto const& type = x.first;
		auto const& system = x.second;
		auto const& systemSignature = m_signatures[type];

		if ((entitySignature & systemSignature) == systemSignature)
		{
			system->m_entities.insert(entity);
		}
		else
		{
			system->m_entities.erase(entity);
		}
	}
}

void Revid::SystemManager::EntityDestroyed(Entity entity)
{
	for (auto const& x : m_systems)
	{
		auto const& system = x.second;
		system->m_entities.erase(entity);
	}
}