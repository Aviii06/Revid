#pragma once
#include <cassert>
#include "ComponentArray.h"
#include "ComponentType.h"
#include "Entity.h"

#include <types/SmartPointers.h>

namespace Revid
{
	class ComponentManager
	{
	public:
		template <typename T>
		void RegisterComponent()
		{
			static ComponentId counter = 0;
			std::string typeName = typeid(T).name();
			m_componentIds.insert({std::string(typeName), counter++});
			m_componentArrays.insert({typeName, std::static_pointer_cast<IComponentArray>(MakeRef<ComponentArray<T>>())});
		}

		template<typename T>
		Ref<ComponentArray<T>> GetComponentArray()
		{
			return std::static_pointer_cast<ComponentArray<T>>(m_componentArrays[typeid(T).name()]);
		}

		template <typename T>
		void AddComponent(Entity entity, T component)
		{
			GetComponentArray<T>()->Insert(entity, component);
		}

		template <typename T>
		void RemoveComponent(Entity entity, T component)
		{
			GetComponentArray<T>()->Remove(entity);
		}

		template <typename T>
		T& GetComponent(Entity entity)
		{
			return GetComponentArray<T>()->Get(entity);
		}

		template <typename T>
		ComponentId GetComponentType()
        {
            String typeName = typeid(T).name();
            auto it = m_componentIds.find(typeName);
            assert(it != m_componentIds.end()  && "Component type not registered");
            return it->second;
        }

		void EntityDestroyed(Entity entity)
		{
			// Notify each component array that an entity has been destroyed
			// If it has a component for that entity, it will remove it
			for (auto const& pair : m_componentArrays)
			{
				auto const& component = pair.second;

				component->EntityDestroyed(entity);
			}
		}

	private:
		Map<String, Ref<IComponentArray>> m_componentArrays{};
		Map<String, ComponentId> m_componentIds{};
	};
}

