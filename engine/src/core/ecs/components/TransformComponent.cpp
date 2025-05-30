#include "TransformComponent.h"

#include "imgui.h"
#include "maths/Vec.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Revid
{
	// void TransformComponent::updateTransformImGUI()
	// {
	// 	if (ImGui::IsKeyPressed((ImGuiKey)90))
	// 		m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
	// 	if (ImGui::IsKeyPressed((ImGuiKey)69))
	// 		m_CurrentGizmoOperation = ImGuizmo::ROTATE;
	// 	if (ImGui::IsKeyPressed((ImGuiKey)82)) // r Key
	// 		m_CurrentGizmoOperation = ImGuizmo::SCALE;
	// 	if (ImGui::RadioButton("Translate", m_CurrentGizmoOperation == ImGuizmo::TRANSLATE))
	// 		m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
	// 	ImGui::SameLine();
	// 	if (ImGui::RadioButton("Rotate", m_CurrentGizmoOperation == ImGuizmo::ROTATE))
	// 		m_CurrentGizmoOperation = ImGuizmo::ROTATE;
	// 	ImGui::SameLine();
	// 	if (ImGui::RadioButton("Scale", m_CurrentGizmoOperation == ImGuizmo::SCALE))
	// 		m_CurrentGizmoOperation = ImGuizmo::SCALE;
	//
	// 	ImGuizmo::DecomposeMatrixToComponents(&m_Transform[0][0], &m_Position.x, &m_Rotation.x, &m_Scale.x);
	// 	ImGui::Text("Position");
	// 	ImGui::SliderFloat3("##Position", &m_Position.x, -500.0f, 500.0f);
	// 	ImGui::Text("Rotation");
	// 	ImGui::SliderFloat3("##Rotation", &m_Rotation.x, -180.0f, 180.0f);
	// 	ImGui::Text("Scale");
	// 	ImGui::SliderFloat3("##Scale", &m_Scale.x, 0.0f, 100.0f, "%.2f");
	// 	ImGui::SameLine();
	// 	ImGui::Text("Fix Scale");
	// 	ImGui::SameLine();
	// 	ImGui::Checkbox("##FixScale", &m_FixScale);
	// 	ImGuizmo::RecomposeMatrixFromComponents(&m_Position.x, &m_Rotation.x, &m_Scale.x, &m_Transform[0][0]);
	//
	// 	if (m_CurrentGizmoOperation != ImGuizmo::SCALE)
	// 	{
	// 		if (ImGui::RadioButton("Local", m_CurrentGizmoMode == ImGuizmo::LOCAL))
	// 			m_CurrentGizmoMode = ImGuizmo::LOCAL;
	// 		ImGui::SameLine();
	// 		if (ImGui::RadioButton("World", m_CurrentGizmoMode == ImGuizmo::WORLD))
	// 			m_CurrentGizmoMode = ImGuizmo::WORLD;
	// 	}
	// 	if (ImGui::IsKeyPressed((ImGuiKey)83))
	// 		m_UsingSnap = !m_UsingSnap;
	// 	ImGui::Checkbox("", &m_UsingSnap);
	// 	ImGui::SameLine();
	//
	// 	switch (m_CurrentGizmoOperation)
	// 	{
	// 	case ImGuizmo::TRANSLATE:
	// 		ImGui::InputFloat3("Snap", &m_Snap[0]);
	// 		break;
	// 	case ImGuizmo::ROTATE:
	// 		ImGui::InputFloat("Angle Snap", &m_Snap[0]);
	// 		break;
	// 	case ImGuizmo::SCALE:
	// 		ImGui::InputFloat("Scale Snap", &m_Snap[0]);
	// 		break;
	// 	}
	// 	//        ImGui::Checkbox("Bound Sizing", &boundSizing);
	// 	//        if (boundSizing)
	// 	//        {
	// 	//            ImGui::PushID(3);
	// 	//            ImGui::Checkbox("", &boundSizingSnap);
	// 	//            ImGui::SameLine();
	// 	//            ImGui::InputFloat3("Snap", boundsSnap);
	// 	//            ImGui::PopID();
	// 	//        }
	// }
	//
	// void TransformComponent::ImGuiRender()
	// {
	// 	Maths::Vec3 prevScale = m_Scale;
	//
	// 	updateTransformImGUI();
	//
	// 	// TODO: This is a hack, fix it
	// 	if (m_FixScale)
	// 	{
	// 		if (m_Scale.x != prevScale.x)
	// 		{
	// 			m_Scale.y = m_Scale.x;
	// 			m_Scale.z = m_Scale.x;
	// 		}
	//
	// 		if (m_Scale.y != prevScale.y)
	// 		{
	// 			m_Scale.x = m_Scale.y;
	// 			m_Scale.z = m_Scale.y;
	// 		}
	//
	// 		if (m_Scale.z != prevScale.z)
	// 		{
	// 			m_Scale.x = m_Scale.z;
	// 			m_Scale.y = m_Scale.z;
	// 		}
	// 	}
	//
	// 	float epsilon = 0.0001f;
	// 	if (m_Scale.x < epsilon)
	// 		m_Scale.x = epsilon;
	// 	if (m_Scale.y < epsilon)
	// 		m_Scale.y = epsilon;
	// 	if (m_Scale.z < epsilon)
	// 		m_Scale.z = epsilon;
	// }
	//
	// void TransformComponent::Draw(Camera* camera)
	// {
	// }
	//
	// void TransformComponent::DrawGizmo(Camera* camera)
	// {
	// 	ImGuizmo::RecomposeMatrixFromComponents(&m_Position.x, &m_Rotation.x, &m_Scale.x, glm::value_ptr(m_Transform));
	// 	ImGuizmo::SetOrthographic(false);
	//
	// 	glm::mat4 cameraProjection = camera->GetProjectionMatrix();
	// 	glm::mat4 cameraView = camera->GetViewMatrix();
	//
	// 	float windowWidth = camera->GetViewportWidth();
	// 	float windowHeight = camera->GetViewportHeight();
	//
	// 	ImGuizmo::SetRect(0, 0, windowWidth, windowHeight);
	//
	// 	ImGuizmo::SetDrawlist();
	// 	ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), m_CurrentGizmoOperation,
	// 	    m_CurrentGizmoMode, glm::value_ptr(m_Transform), NULL, NULL);
	//
	// 	ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(m_Transform), &m_Position.x, &m_Rotation.x, &m_Scale.x);
	// }
}