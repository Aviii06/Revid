#pragma once
#include <types/SmartPointers.h>
#include <revid_engine/core/ecs/ComponentManager.h>
#include <revid_engine/core/ecs/EntityManager.h>
#include <revid_engine/core/ecs/SystemManager.h>

namespace Revid
{
	class Coordinator
	{
	public:
		void Init()
		{
			// Create pointers to each manager
			m_componentManager = MakePtr<ComponentManager>();
			m_entityManager = MakePtr<EntityManager>();
			m_systemManager = MakePtr<SystemManager>();
		}


		// Entity methods
		Entity CreateEntity()
		{
			return m_entityManager->CreateEntity();
		}

		void DestroyEntity(Entity entity)
		{
			m_entityManager->DeleteEntity(entity);

			m_componentManager->EntityDestroyed(entity);

			m_systemManager->EntityDestroyed(entity);
		}


		// Component methods
		template <typename T>
		void RegisterComponent()
		{
			m_componentManager->RegisterComponent<T>();
		}

		template <typename T>
		void AddComponent(Entity entity, T component)
		{
			m_componentManager->AddComponent<T>(entity, component);

			auto signature = m_entityManager->GetSignature(entity);
			signature.set(m_componentManager->GetComponentType<T>(), true);
			m_entityManager->SetSignature(entity, signature);

			m_systemManager->EntitySignatureChanged(entity, signature);
		}

		template <typename T>
		void RemoveComponent(Entity entity)
		{
			m_componentManager->RemoveComponent<T>(entity);

			auto signature = m_entityManager->GetSignature(entity);
			signature.set(m_componentManager->GetComponentType<T>(), false);
			m_entityManager->SetSignature(entity, signature);

			m_systemManager->EntitySignatureChanged(entity, signature);
		}

		template <typename T>
		T& GetComponent(Entity entity)
		{
			return m_componentManager->GetComponent<T>(entity);
		}

		template <typename T>
		ComponentId GetComponentType()
		{
			return m_componentManager->GetComponentType<T>();
		}


		// System methods
		template <typename T>
		Ref<T> RegisterSystem()
		{
			return m_systemManager->RegisterSystem<T>();
		}

		template <typename T>
		void SetSystemSignature(Signature signature)
		{
			m_systemManager->SetSignature<T>(signature);
		}

		template <typename T>
		Ref<T> GetSystem()
		{
			return m_systemManager->GetSystem<T>();
		}

	private:
		Ptr<ComponentManager> m_componentManager;
		Ptr<EntityManager> m_entityManager;
		Ptr<SystemManager> m_systemManager;
	};
}
