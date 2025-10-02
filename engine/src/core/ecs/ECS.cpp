#include "revid_engine/core/ecs/ECS.h"

namespace Revid
{
	Ptr<EntityManager> g_entity_manager;
	Ptr<ComponentManager> g_component_manager;
	Ptr<SystemManager> g_system_manager;

	void Init()
	{
		g_entity_manager = MakePtr<EntityManager>();
		g_component_manager = MakePtr<ComponentManager>();
		g_system_manager = MakePtr<SystemManager>();
	}
}





// #include "ECS.h"
// #include "ComponentFactory.h"
//
// Map<int, Ref<Revid::Component>> g_Components;
// Map<int, Ref<Revid::Entity>> g_Entities;
//
// int Revid::ECS::s_EntityID = 0;
// int Revid::ECS::s_ComponentID = 0;
//
// bool Revid::ECS::AddComponent(int componentID, int entityID)
// {
// 	// Check if the component already exists
// 	// If So then just update the component
// 	Ref<Component> component = g_Components[componentID];
// 	const ComponentType ct = component->GetComponentType();
//
// 	const int index = g_Entities[entityID]->HasComponent(ct);
// 	Ref<Entity> entity = g_Entities[entityID];
// 	if (index != -1)
// 	{
// 		const String& componentName = g_AllComponentStrings.at(ct);
// 		std::cerr << componentName << " already exists\n"
// 		          << std::endl;
// 		std::cout << "Updating component: " << componentName << std::endl;
//
// 		entity->RemoveComponent(componentID);
// 	}
//
// 	component->SetEntity(entity->GetEntityID());
//
// 	g_Components[componentID] = component;
// 	entity->AddComponent(componentID);
// 	return true;
// }
//
// bool Revid::ECS::RemoveComponent(int componentID, int entityID)
// {
// 	Ref<Component> component = g_Components[componentID];
// 	const ComponentType ct = component->GetComponentType();
//
// 	const int index = g_Entities[entityID]->HasComponent(ct);
// 	Ref<Entity> entity = g_Entities[index];
// 	component->SetEntity(entity->GetEntityID());
//
// 	for (auto&& comp : g_Components)
// 	{
// 		if (comp.second->GetComponentType() == component->GetComponentType())
// 		{
// 			entity->RemoveComponent(componentID);
// 			auto it = g_Components.find(comp.first);
// 			g_Components.erase(it);
// 			return true;
// 		}
// 	}
//
// 	const String& componentName = g_AllComponentStrings.at(ct);
// 	std::cerr << "Failed to remove component: " << componentName << std::endl;
// 	return false;
// }
//
// void Revid::ECS::Draw(Camera* camera)
// {
// 	for (auto& entity : g_Entities)
// 	{
// 		entity.second->Draw(camera);
// 	}
// }
//
// void Revid::ECS::ImGuiRender()
// {
// 	for (auto& entity : g_Entities)
// 	{
// 		entity.second->ImguiRender();
// 	}
// }
//
// Ref<Revid::Entity> Revid::ECS::CreateEntity(const String& name)
// {
// 	Ref<Entity> entity = MakeRef<Entity>(s_EntityID, name);
// 	g_Entities[entity->GetEntityID()] = entity;
//
// 	// std::cout << g_Entities[entity->GetID()] << "\n";
// 	// std::cout << entity << "\n";
//
// 	auto tc = ECS::CreateComponent<TransformComponent>();
// 	AddComponent(tc->GetComponentID(), entity->GetEntityID());
// 	return entity;
// }
//
// Ref<Revid::Component> Revid::ECS::GetComponent(ComponentType ct, int entityID)
// {
// 	int componentID = g_Entities[entityID]->HasComponent(ct);
//
// 	if (componentID == -1)
// 	{
// 		return nullptr;
// 	}
// 	return g_Components[componentID];
// }
