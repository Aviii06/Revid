#pragma once
#include <types/SmartPointers.h>
#include <revid_engine/core/ecs/ComponentManager.h>
#include <revid_engine/core/ecs/EntityManager.h>
#include <revid_engine/core/ecs/SystemManager.h>

namespace Revid
{
	template <typename... ComponentTypes>
	class View
	{
	public:
		View(std::vector<Entity> entities)
			: m_entities(std::move(entities))
		{
		}

		std::vector<Entity>::iterator begin() { return m_entities.begin(); }
		std::vector<Entity>::iterator end() { return m_entities.end(); }

	private:
		std::vector<Entity> m_entities;
	};

	class ECSRegistry
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

		template<typename T>
		bool HasComponent(Entity entity)
		{
			return m_componentManager->HasComponent<T>(entity);
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

		// Get entities of a particular system
		template <typename SystemType>
		Set<Entity> GetEntitiesWithSystem()
		{
			auto system = m_systemManager->GetSystem<SystemType>();
			if (system)
			{
				return system->m_entities;
			}

			return Set<Entity>();
		}

		// Get Entities having all the components
		template<typename... ComponentTypes>
		View<ComponentTypes...> View()
		{
			std::vector<Entity> result;

			for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
			{
				if ((HasComponent<ComponentTypes>(entity) && ...))
				{
					result.push_back(entity);
				}
			}

			return View<ComponentTypes...>(std::move(result));
		}

	private:
		Ptr<ComponentManager> m_componentManager;
		Ptr<EntityManager> m_entityManager;
		Ptr<SystemManager> m_systemManager;
	};
}
