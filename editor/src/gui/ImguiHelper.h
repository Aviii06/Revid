#pragma once


ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
ImGuiIO& io = ImGui::GetIO();
namespace RevidEditor
{
	static void glfw_error_callback(int error, const char* description)
	{
		fprintf(stderr, "GLFW Error %d: %s\n", error, description);
	}

	int SetupImgui()
	{

	}

	void UpdateImgui()
	{

	}
}
