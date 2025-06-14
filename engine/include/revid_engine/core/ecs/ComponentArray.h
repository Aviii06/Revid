#pragma once
#include <types/Containers.h>
#include <iostream>
#include "Entity.h"


namespace Revid
{
	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(Entity entity) = 0;
	};

	template <typename T>
	class ComponentArray : public IComponentArray
	{
	private:
		Array<T, MAX_ENTITIES> m_components;
		Map<Entity, size_t> m_entityToIndex;
		Map<size_t, Entity> m_indexToEntity;
		size_t m_size;

	public:
		ComponentArray()
		{
			m_size = 0;
		}

		Vector<T> GetComponents() { return m_components; }

		void Insert(Entity entity, T component)
		{
			if (m_entityToIndex.find(entity) == m_entityToIndex.end())
			{
				std::cerr << "Cannot add the same component to the same entity. EntityID: " << entity << std::endl;
				return;
			}

			m_components[entity] = component;
			m_entityToIndex[entity] = m_size;
			m_indexToEntity[m_size] = entity;
			m_size++;
		}

		void Remove(Entity entity)
		{
			if (m_entityToIndex.find(entity) != m_entityToIndex.end())
			{
				std::cerr << "The entity with EntityID: " << entity << " doesn't contain the component" << std::endl;
				return;
			}

			size_t removedEntityIndex = m_entityToIndex[entity];
			size_t lastElementIndex = m_size - 1;

			m_components[removedEntityIndex] = m_components[lastElementIndex];

			Entity last_entity_id = m_indexToEntity[lastElementIndex];
			m_entityToIndex[last_entity_id] = removedEntityIndex;
			m_indexToEntity[removedEntityIndex] = last_entity_id;

			// m_components.pop_back();
			m_entityToIndex.erase(entity);
			m_indexToEntity.erase(lastElementIndex);

			--m_size;
		}

		T& Get(Entity entity)
		{
			if (m_entityToIndex.find(entity) != m_entityToIndex.end())
			{
				std::cerr << "Couldn't get the component of the entity with EntityID: " << entity << std::endl;
			}

			return m_components[m_entityToIndex[entity]];
		}

		void EntityDestroyed(Entity entity) override
		{
			if (m_entityToIndex.find(entity) != m_entityToIndex.end())
			{
				Remove(entity);
			}
		}

	};

}
