#pragma once
#include "System.h"
#include "SystemType.h"
#include "types/SmartPointers.h"

namespace Revid
{
	class SystemManager
	{
	private:
		Map<SystemType, Ref<System>> m_systems;
		Map<SystemType, Signature> m_signatures;
	public:
		template <typename T>
		void SetSignature(Signature signature)
		{
			m_signatures[T::GetType()] = signature;
		}

		void EntitySignatureChanged(Entity entity)
		{
			auto const& entitySignature = entity.GetSignature();

			for (auto const& x : m_systems)
			{
				auto const& type = x.first;
				auto const& system = x.second;
				auto const& systemSignature = m_signatures[type];

				if ((entitySignature & systemSignature) != systemSignature)
				{
					size_t size = system->m_entities.size();

					// TODO: Shift to a more data rustlike approach.
					for (int i = 0; i < size; i++)
					{
						if (system->m_entities[i].GetID() == entity.GetID())
						{
							system->m_entities[i] = system->m_entities[size - 1];
							system->m_entities.pop_back();
							break;
						}
					}
				}

				system->m_entities.push_back(entity);
			}
		}
	};
}
