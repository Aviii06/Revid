#pragma once

namespace RevidEditor
{
    void ViewportPane()
    {
        auto sceneTextureID = ImGui_ImplVulkan_AddTexture(
            Revid::ServiceLocator::GetRenderer()->GetSceneSampler(),
            Revid::ServiceLocator::GetRenderer()->GetSceneImageView(),
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );

        Revid::ServiceLocator::GetRenderer()->SetCurrentDescSet(sceneTextureID);

        ImGui::Begin("Viewport");

		int fb_width, fb_height;
		GLFWwindow* window = Revid::ServiceLocator::GetWindow()->GetGLFWWindow();
		glfwGetFramebufferSize(window, &fb_width, &fb_height);

        ImVec2 viewportPanelSize = ImVec2(fb_width, fb_height);

        // Maintain aspect ratio if needed
        ImGui::Image(
            Revid::ServiceLocator::GetRenderer()->GetCurrentDescSet(),
            viewportPanelSize,
            ImVec2(0, 0), // UV top-left
            ImVec2(1, 1) // UV bottom-right
        );

        ImGui::End();
    }
}

