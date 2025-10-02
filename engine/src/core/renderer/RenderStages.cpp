#include <revid_engine/core/renderer/RenderStages.h>
#include <revid_engine/ServiceLocator.h>
#include <revid_engine/core/ecs/components/TransformComponent.h>
#include <revid_engine/core/ecs/components/model/ModelComponent.h>

#include "glm/gtx/string_cast.hpp"

void Revid::GBufferRS::Record(RenderGraph& graph)
{
	auto ent = ServiceLocator::GetECSRegistry()->View<TransformComponent, ModelComponent>();

	for (auto entity : ent)
	{
		ModelComponent& modelComponent = ServiceLocator::GetECSRegistry()->GetComponent<ModelComponent>(entity);
		TransformComponent& transformComponent = ServiceLocator::GetECSRegistry()->GetComponent<TransformComponent>(entity);

		for (auto& mesh: modelComponent.m_meshes)
		{
			// Update UBO.
			UniformBufferObject ubo{};
			ubo.model = transformComponent.GetTransform();

			ubo.proj = ServiceLocator::GetCamera()->GetProjectionMatrix();
			ubo.view = ServiceLocator::GetCamera()->GetViewMatrix();
			ubo.proj[1][1] *= -1;

			auto rc = [=](VkCommandBuffer cmd, uint32_t frameIndex)
			{
				vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, mesh->GetPipelineInfo()->GetPipeline());
				mesh->GetPipelineInfo()->UpdateUniformBuffer(ubo, frameIndex);
				VkDeviceSize offset = 0;
				vkCmdBindVertexBuffers(cmd, 0, 1, mesh->GetVertexBuffer().get(), &offset);
				vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, mesh->GetPipelineInfo()->GetPipelineLayout(), 0, 1,
												mesh->GetPipelineInfo()->GetDescriptorSet(frameIndex), 0, nullptr);

				VkBuffer indexBuffer = mesh->GetIndexBuffer();
				int indexCount = mesh->GetIndicesSize();
				vkCmdBindIndexBuffer(cmd, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
				vkCmdDrawIndexed(cmd, static_cast<uint32_t>(indexCount), mesh->GetInstanceCount(), 0, 0,
								 0);
			};

			graph.SubmitCommand(0, 0, rc);
		}
	}
}

void Revid::LightingRS::Record(RenderGraph& graph)
{
	auto rc = [&](VkCommandBuffer commandBuffer, uint32_t frameIndex)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ServiceLocator::GetRenderer()->GetLightingPipeline());

		VkBuffer vertexBuffers2[] = {ServiceLocator::GetRenderer()->GetLightingVertexBuffer()};
		VkDeviceSize offsets2[] = {0};
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers2, offsets2);
		vkCmdBindIndexBuffer(commandBuffer, ServiceLocator::GetRenderer()->GetLightingIndexBuffer(), 0, VK_INDEX_TYPE_UINT16);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ServiceLocator::GetRenderer()->GetLightingPipelineLayout(), 0, 1, ServiceLocator::GetRenderer()->GetLightingDescriptorSet(frameIndex), 0, nullptr);
		// TODO: The 6 has to be m_indices.size()
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(6), 1, 0, 0, 0);
	};

	graph.SubmitCommand(0, 1, rc);
}
