#pragma once
#include "ComponentArray.h"
#include "ComponentType.h"
#include "Entity.h"

#include <types/SmartPointers.h>

namespace Revid
{
	class ComponentManager
	{
	private:
		Map<ComponentType, Ref<IComponentArray>> m_componentArrays;

	public:
		template<typename T>
		Ref<ComponentArray<T>> GetComponentArray()
		{
			return std::static_pointer_cast<ComponentArray<T>>(m_componentArrays[T::GetComponentType()]);
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
	};
}

