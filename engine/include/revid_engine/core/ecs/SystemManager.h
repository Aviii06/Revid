#pragma once
#include "System.h"
#include "types/SmartPointers.h"

namespace Revid
{
	class SystemManager
	{
	public:
		template <typename T>
		void SetSignature(Signature signature)
		{
			const char* typeName = typeid(T).name();
			m_signatures.insert({typeName, signature});
		}

		template <typename T>
		Ref<T> RegisterSystem()
		{
			const char* typeName = typeid(T).name();
			Ref<T> system = MakeRef<T>();
			m_systems.insert({typeName, system});
			return system;
		}

		template <typename T>
		Ref<T> GetSystem()
		{
			const char* typeName = typeid(T).name();
			auto it = m_systems.find(typeName);
			if (it != m_systems.end())
			{
				return it->second;
			}
			return nullptr;
		}

		void EntitySignatureChanged(Entity entity, Signature entitySignature);
		void EntityDestroyed(Entity entity);

	private:
		Map<const char*, Ref<System>> m_systems;
		UnorderedMap<const char*, Signature> m_signatures;
	};
}
