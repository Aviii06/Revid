#pragma once

#include "types/SmartPointers.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"

namespace Revid
{
	namespace ECS
	{
		static Ptr<EntityManager> g_entity_manager = MakePtr<EntityManager>();
		static Ptr<ComponentManager> g_component_manager = MakePtr<ComponentManager>();
		static Ptr<SystemManager> g_system_manager = MakePtr<SystemManager>();

		void Init();

		Entity CreateEntity()
		{
			Entity ent = g_entity_manager->CreateEntity();
			std::cout << "hello";
			return ent;
		}

		template <typename T>
		void AddComponent(Entity entity, T component)
		{
			g_component_manager->AddComponent<T>(entity, component);

			Signature signature = entity.GetSignature();
			signature.set((size_t)T::GetComponentType());
			entity.SetSignature(signature);

			g_system_manager->EntitySignatureChanged(entity);
		}

		template <typename T>
		Vector<T> GetComponents()
		{
			return g_component_manager->GetComponentArray<T>()->GetComponents();
		}

		template <typename T>
		void RemoveComponent(Entity entity)
		{
			// TODO
		}
	}

	// /*!
	//  * @namespace ECS
	//  * @brief Contains functions to create and manage entities and components.
	//  * @details ECS is a namespace that contains functions to create and manage entities and components.
	//  * It contains a global map of entities and components. g_Entities is a map that contains all the entities,
	//  * it is a map from the entity ID to the shared pointer of the entity, and g_Components is a map that contains
	//  * all the components, it is a map from the component ID to the shared pointer of the component.
	//  */
	// namespace ECS
	// {
	// 	extern Map<int, Ref<Component>> g_Components;
	// 	extern Map<int, Ref<Entity>> g_Entities;
	// 	extern int s_EntityID;
	// 	extern int s_ComponentID;
	//
	// 	bool AddComponent(int componentID, int entityID);
	// 	bool RemoveComponent(int componentID, int entityID);
	//
	// 	void Draw(Camera* camera);
	// 	void ImGuiRender();
	//
	// 	/*!
	// 	 * @brief Gets the Component of particular entity.
	// 	 * @param entityID The ID of the entity.
	// 	 * @param ct The type of the component.
	// 	 * @return The component, nullptr if not found.
	// 	 */
	// 	Ref<Component> GetComponent(ComponentType ct, int entityID);
	//
	// 	/*!
	// 	 * @brief Gets All the Components of particular entity.
	// 	 * @details It takes in the ID of the entity and a vector of components. It then reserves the size of the vector
	// 	 * to the size of the components of the entity. Fills the vector with the components of the entity. The vector is
	// 	 * passed by reference.
	// 	 * @param ct The type of the component.
	// 	 * @param components The vector of components. Passed by reference.
	// 	 * @tparam The type of the component.
	// 	 */
	// 	template <typename T>
	// 	void GetAllComponents(const ComponentType ct, Vector<T*>& components)
	// 	{
	// 		components.reserve(g_Components.size());
	// 		for (auto& component : g_Components)
	// 		{
	// 			if (component.second->GetComponentType() == ct)
	// 			{
	// 				components.emplace_back(static_cast<T*>(component.second.get()));
	// 			}
	// 		}
	// 	}
	//
	// 	/*!
	// 	 * @brief Creates an Entity.
	// 	 * @param name The name of the entity
	// 	 * @return The shared pointer of the entity.
	// 	 */
	// 	Ref<Entity> CreateEntity(const String& name);
	//
	// 	/*!
	// 	 * @brief Creates a Component.
	// 	 * @details It takes in the type of the component and returns the shared pointer of the component.
	// 	 * @tparam T The type of the component or derived classes.
	// 	 * @return The shared pointer of the component.
	// 	 */
	// 	template <typename T>
	// 	typename std::enable_if<std::is_base_of<Component, T>::value, Ref<T>>::type CreateComponent()
	// 	{
	// 		Ref<T> component = MakeRef<T>();
	// 		g_Components[component->GetComponentID()] = component;
	// 		return component;
	// 	}
	// };
}
