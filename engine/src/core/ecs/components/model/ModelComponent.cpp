#include "imgui.h"
#include "revid_engine/core/ecs/components/TransformComponent.h"
#include "revid_engine/core/ecs/ECS.h"

#define MAX_MESHES 10

// Revid::ModelComponent::ModelComponent()
// {
// 	m_Meshes.reserve(MAX_MESHES);
// }
//
// void Revid::ModelComponent::Draw(Camera* camera)
// {
// 	auto component = ECS::GetComponent(ComponentType::TransformComponent, m_OwnerEntityID);
// 	if (!component)
// 	{
// 		std::cerr << "Entity does not have a TransformComponent\n";
// 		return;
// 	}
//
// 	auto transformComponent = dynamic_cast<TransformComponent*>(component.get());
// 	glm::mat4 transform = transformComponent->GetTransform();
//
// 	for (auto& mesh : m_Meshes)
// 	{
// 		mesh->Update(transform);
// 		mesh->Draw(camera);
// 	}
// }
//
// void Revid::ModelComponent::AddMesh(Revid::Mesh* mesh)
// {
// 	m_Meshes.push_back(mesh);
// }
//
// void Revid::ModelComponent::RemoveMesh(Revid::Mesh* mesh)
// {
// 	m_Meshes.erase(std::remove(m_Meshes.begin(), m_Meshes.end(), mesh), m_Meshes.end());
// }
//
// void Revid::ModelComponent::ImGuiRender()
// {
// 	static ImGuiTableColumnFlags columns_base_flags = ImGuiTableColumnFlags_None;
//
// 	if (ImGui::BeginTable("table_advanced", 4, 0, ImVec2(0, 0), 0))
// 	{
// 		ImGui::TableSetupColumn("ID", columns_base_flags, 0.0f, 0);
// 		ImGui::TableSetupColumn("Action-Remove", columns_base_flags | ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, 1);
// 		ImGui::TableSetupColumn("Action-Edit", columns_base_flags | ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, 2);
//
// 		ImGui::PushButtonRepeat(true);
// 		ImGuiListClipper clipper;
// 		clipper.Begin(m_Meshes.size());
// 		int row_minimum_height = (int)10;
// 		while (clipper.Step())
// 		{
// 			for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
// 			{
// 				if (row_n >= m_Meshes.size())
// 					break;
// 				Mesh* item = m_Meshes[row_n];
//
// 				ImGui::PushID(item->GetID());
// 				ImGui::TableNextRow(ImGuiTableRowFlags_None, row_minimum_height);
//
// 				ImGui::TableSetColumnIndex(0);
// 				ImGui::Text("Mesh %d", item->GetID());
//
// 				if (ImGui::TableSetColumnIndex(1))
// 				{
// 					if (ImGui::ImageButton((ImTextureID)RevidGui::Assets::GetInstance()->GetTexMinus()->GetRendererID(), ImVec2(10, 10),
// 					        ImVec2(0.25, 0.25), ImVec2(0.75, 0.75), 2))
// 					{
// 						RemoveMesh(m_Meshes[row_n]);
// 					}
// 				}
//
// 				if (ImGui::TableSetColumnIndex(2))
// 				{
// 					if (ImGui::ImageButton((ImTextureID)RevidGui::Assets::GetInstance()->GetTexEdit()->GetRendererID(), ImVec2(10, 10),
// 					        ImVec2(0.25, 0.25), ImVec2(0.75, 0.75), 2))
// 					{
// 						m_Meshes[row_n]->EditMesh();
// 					}
// 				}
// 				ImGui::PopID();
// 			}
// 		}
// 		ImGui::PopButtonRepeat();
// 		ImGui::EndTable();
//
// 		if (ImGui::ImageButton((ImTextureID)RevidGui::Assets::GetInstance()->GetTexPlus()->GetRendererID(), ImVec2(10, 10),
// 		        ImVec2(0.25, 0.25), ImVec2(0.75, 0.75), 2))
// 		{
// 			std::string file = FileDialogue::OpenFile({}, {});
// 			if (!file.empty())
// 			{
// 				Mesh* mesh = new Mesh(file);
// 				AddMesh(mesh);
// 			}
// 		}
//
// 		for (auto&& mesh : m_Meshes)
// 		{
// 			mesh->ImGuiRender();
// 		}
// 	}
// }