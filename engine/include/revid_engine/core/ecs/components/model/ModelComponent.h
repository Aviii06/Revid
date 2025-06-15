#pragma once

#include "revid_engine/core/ecs/Component.h"
#include "types/Containers.h"
#include "types/SmartPointers.h"
#include "revid_engine/core/renderer/Mesh.h"

namespace Revid
{
	// struct ModelComponent : public
	// {
	// 	Vector<Ref<Mesh>> m_meshes;
	// };

	// /*!
	//  * @class ModelComponent
	//  * @brief Contains a ModelComponent.
	//  * @details ModelComponent is a class that contains a model component. It is a derived class from Component.
	//  * It contains a vector of meshes. These meshes are then rendered in the scene. It also contains a UI for the model component.
	//  * From the UI you can add and remove meshes from the model component.
	//  */
	// class ModelComponent : public Component
	// {
	// private:
	// 	Vector<Mesh*> m_Meshes;
	//
	// public:
	// 	ModelComponent();
	// 	virtual ~ModelComponent() = default;
	//
	// 	void ImGuiRender() override;
	// 	void Draw(Camera* camera) override;
	//
	// 	void AddMesh(Revid::Mesh* mesh);
	// 	void RemoveMesh(Revid::Mesh* mesh);
	//
	// 	inline Vector<Revid::Mesh*> GetMeshes() { return m_Meshes; }
	// 	ComponentType GetComponentType() override { return ComponentType::ModelComponent; }
	// };

	struct ModelComponent
	{
		Vector<Ref<Mesh>> m_Meshes;
	};
}
