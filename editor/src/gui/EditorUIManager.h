#pragma once
#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>
#include <revid_engine/ServiceLocater.h>
#include <gui/dockspace/DockspaceLayout.h>
#include <gui/configs/UIFlags.h>

namespace RevidEditor
{
	static ImVec4 m_clearColor;
	static void coreSetup();

	void SetupImgui()
	{
		coreSetup();
		ImGuiThemeSetup();
	}

	void UpdateImgui()
	{
		glfwPollEvents();

		int fb_width, fb_height;
		GLFWwindow* window = Revid::ServiceLocator::GetWindow()->GetGLFWWindow();
		glfwGetFramebufferSize(window, &fb_width, &fb_height);

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		RenderDockspace();
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)
		// ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		// ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&m_clearColor); // Edit 3 floats representing a color

		if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		// ImGui_ImplVulkan_RemoveTexture(Revid::ServiceLocator::GetRenderer()->GetCurrentDescSet());
		auto sceneTextureID = ImGui_ImplVulkan_AddTexture(
			Revid::ServiceLocator::GetRenderer()->GetSceneSampler(),                // Any basic sampler will do
			Revid::ServiceLocator::GetRenderer()->GetSceneImageView(), // The image view of the scene color attachment
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		);

		Revid::ServiceLocator::GetRenderer()->SetCurrentDescSet(sceneTextureID);

		ImGui::Begin("Viewport");

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		// Maintain aspect ratio if needed
		ImGui::Image(
			Revid::ServiceLocator::GetRenderer()->GetCurrentDescSet(),
			viewportPanelSize,
			ImVec2(0, 0),     // UV top-left
			ImVec2(1, 1)      // UV bottom-right
		);

		ImGui::End();

		// Ends the dock
		ImGui::End();

		ImGui::Render();
	}

	static void coreSetup()
	{
		int w, h;
		GLFWwindow* window = Revid::ServiceLocator::GetWindow()->GetGLFWWindow();
		glfwGetFramebufferSize(window, &w, &h);

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		Revid::ServiceLocator::GetRenderer()->CreateImguiDescriptorPool();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		io.Fonts->AddFontDefault();
		// io.Fonts->Build();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

		// Setup Dear ImGui style
		// ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForVulkan(window, true);
		ImGui_ImplVulkan_InitInfo init_info = Revid::ServiceLocator::GetRenderer()->GetInitInfo();

		ImGui_ImplVulkan_Init(&init_info);

		// Our state
		bool show_demo_window = true;
		bool show_another_window = false;

		m_clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}


}



