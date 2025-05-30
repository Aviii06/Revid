#pragma once
#include "maths/Vec.h"
#include "core/ecs/Component.h"

namespace Revid
{
	struct TransformComponent : public Component<ComponentType::TransformComponent>
	{
		Maths::Vec3 m_position;
		Maths::Vec3 m_rotation;
		Maths::Vec3 m_scale;
	};
}




// #include "core/ecs/Component.h"
// #include "common/maths/Vec.h"
// #include "imguizmo/ImGuizmo.h"
//
// #include "glm/gtc/quaternion.hpp"

// namespace Revid
// {
//
// 	/*!
// 	 * @class TransformComponent
// 	 * @brief Contains the Transform Component.
// 	 * @details TransformComponent is a class that contains the Transform Component. It contains the position, rotation, and scale of the entity.
// 	 * This component is a must have for every entity. It also contains the transform matrix of the entity. It also contains the gizmo operation,
// 	 * gizmo mode, and snap values for the gizmo.
// 	 */
// 	class TransformComponent : public Component
// 	{
// 	private:
// 		Maths::Vec3 m_Position;
// 		Maths::Vec3 m_Rotation;
// 		Maths::Vec3 m_Scale;
// 		Maths::Vec3 m_PrevScale = Maths::Vec3(50.0f, 50.0f, 50.0f);
// 		bool m_FixScale = false;
// 		glm::mat4 m_Transform;
// 		ImGuizmo::OPERATION m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
// 		ImGuizmo::MODE m_CurrentGizmoMode = ImGuizmo::LOCAL;
// 		bool m_UsingSnap = false;
// 		float m_Snap[3] = { 1.f, 1.f, 1.f };
//
// 		void updateTransformImGUI();
//
// 		void drawGizmo(Camera* camera);
//
// 	public:
// 		TransformComponent()
// 		{
// 			m_Scale = Maths::Vec3(1.0f, 1.0f, 1.0f);
// 			m_Position = Maths::Vec3(0.0f, 0.0f, 0.0f);
// 			m_Rotation = Maths::Vec3(0.0f, 0.0f, 0.0f);
// 			ImGuizmo::RecomposeMatrixFromComponents(&m_Position.x, &m_Rotation.x, &m_Scale.x, &m_Transform[0][0]);
// 		}
// 		TransformComponent(Maths::Vec3 position, Maths::Vec3 rotation, Maths::Vec3 scale)
// 		    : m_Position(position)
// 		    , m_Rotation(rotation)
// 		    , m_Scale(scale)
// 		{
// 			ImGuizmo::RecomposeMatrixFromComponents(&m_Position.x, &m_Rotation.x, &m_Scale.x, &m_Transform[0][0]);
// 		}
// 		virtual ~TransformComponent() = default;
//
//
// 		inline Maths::Vec3 GetPosition() const { return m_Position; }
// 		inline Maths::Vec3 GetRotation() const { return m_Rotation; }
// 		inline Maths::Vec3 GetScale() const { return m_Scale; }
// 		inline glm::mat4 GetTransform() { return m_Transform; }
//
// 		inline void SetPosition(Maths::Vec3 position)
// 		{
// 			m_Position = position;
// 			ImGuizmo::RecomposeMatrixFromComponents(&m_Position.x, &m_Rotation.x, &m_Scale.x, &m_Transform[0][0]);
// 		}
// 		inline void SetRotation(Maths::Vec3 rotation)
// 		{
// 			m_Rotation = rotation;
// 			ImGuizmo::RecomposeMatrixFromComponents(&m_Position.x, &m_Rotation.x, &m_Scale.x, &m_Transform[0][0]);
// 		}
// 		inline void SetScale(Maths::Vec3 scale)
// 		{
// 			m_Scale = scale;
// 			ImGuizmo::RecomposeMatrixFromComponents(&m_Position.x, &m_Rotation.x, &m_Scale.x, &m_Transform[0][0]);
// 		}
//
// 		ComponentType GetComponentType() override { return ComponentType::TransformComponent; }
//
// 		inline ImGuizmo::OPERATION GetCurrentGizmoOperation() const { return m_CurrentGizmoOperation; }
// 		inline ImGuizmo::MODE GetCurrentGizmoMode() const { return m_CurrentGizmoMode; }
// 		inline bool IsUsingSnap() const { return m_UsingSnap; }
// 		inline float* GetSnap() { return m_Snap; }
//
// 		void DrawGizmo(Camera* camera);
// 		void Draw(Camera* camera) override;
// 		void ImGuiRender() override;
// 	};
// }
