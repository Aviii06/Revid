#pragma once
#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>
#include <revid_engine/ServiceLocator.h>
#include <gui/dockspace/DockspaceLayout.h>
#include <gui/configs/UIFlags.h>

#include <gui/panes/ViewportPane.h>

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

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		BeginDockspace();

		ViewportPane();

		EndDockspace();

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



