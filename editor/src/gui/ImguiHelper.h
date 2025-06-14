#pragma once
#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>
#include <revid_engine/ServiceLocater.h>

namespace RevidEditor
{
	static ImVec4 m_clearColor;
	static ImGuiIO m_imguiIo;

	void SetupImgui()
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
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForVulkan(window, true);
		ImGui_ImplVulkan_InitInfo init_info = Revid::ServiceLocator::GetRenderer()->GetInitInfo();

		ImGui_ImplVulkan_Init(&init_info);

		// Our state
		bool show_demo_window = true;
		bool show_another_window = false;

		m_clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		m_imguiIo = ImGui::GetIO();
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
		{
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

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_imguiIo.Framerate, m_imguiIo.Framerate);
			ImGui::End();
		}
		ImGui::Render();
	}
}
