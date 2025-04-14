#include "Application.h"
#include "revid_engine/ServiceLocater.h"
#include <revid_engine/platform/CustomWindow.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/transform.hpp"
#include "revid_engine/renderer/vulkan/VulkanRenderer.h"
#include "logging/Logging.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

RevidEditor::Application::Application()
    : Application("Revid")
{
}

RevidEditor::Application::Application(String title)
    : m_title(std::move(title)), m_isRunning(true)
{
    Revid::Logger::Log(Revid::LogLevel::INFO, "Initialising Revid Application named: " + title);
    initializeLogger();
    intializeServices();

	m_clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	m_imguiIo = ImGui::GetIO();

}

RevidEditor::Application::~Application()
{
    shutdownServices();
}

void RevidEditor::Application::Run()
{
    while(m_isRunning)
    {
        if (Revid::ServiceLocator::GetWindow()->Update())
        {
            m_isRunning = false;
            continue;
        }

    	Revid::ServiceLocator::GetInputHandler()->HandleInput();

		// Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Resize swap chain?
        int fb_width, fb_height;
		GLFWwindow* window = Revid::ServiceLocator::GetWindow()->GetGLFWWindow();
        glfwGetFramebufferSize(window, &fb_width, &fb_height);
        // if (fb_width > 0 && fb_height > 0 && (g_SwapChainRebuild || g_MainWindowData.Width != fb_width || g_MainWindowData.Height != fb_height))
        // {
        //     ImGui_ImplVulkan_SetMinImageCount(g_MinImageCount);
        //     ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance, g_PhysicalDevice, g_Device, &g_MainWindowData, g_QueueFamily, g_Allocator, fb_width, fb_height, g_MinImageCount);
        //     g_MainWindowData.FrameIndex = 0;
        //     g_SwapChainRebuild = false;
        // }
        // if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        // {
        //     ImGui_ImplGlfw_Sleep(10);
        //     continue;
        // }

        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //
        // // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        // // if (show_demo_window)
        // //     ImGui::ShowDemoWindow(&show_demo_window);
        //
        // // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            // ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            // ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&m_clearColor); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_imguiIo.Framerate, m_imguiIo.Framerate);
            ImGui::End();
        }
        //
        // // Rendering
        ImGui::Render();
    	Revid::ServiceLocator::GetRenderer()->Render();
        // ImDrawData* draw_data = ImGui::GetDrawData();
        // const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
    }
}

void RevidEditor::Application::intializeServices()
{
    Revid::ServiceLocator::Provide(new Revid::CustomWindow());

    Revid::ServiceLocator::GetWindow()->OpenWindow({
        m_title,
        1920,
        1080
    });

	Revid::ServiceLocator::Provide(new Revid::EditorCamera());

    Revid::RendererSettings settings {
    	3,
          m_title,
        Revid::ServiceLocator::GetWindow()->GetExtentions(),
        Revid::ServiceLocator::GetWindow()->GetExtentionCount()
    };

    Revid::ServiceLocator::Provide(new Revid::VulkanRenderer(), settings);

	Revid::ServiceLocator::Provide(new Revid::InputHandler());

	Ref<Revid::Mesh> mesh = MakeRef<Revid::Mesh>("./assets/obj/grass.obj");
	mesh->SetInstanceCount(2e4);
	glm::mat4 modelMatrix2 = glm::mat4(1.0f);
	mesh->SetModelMatrix(modelMatrix2);
	Revid::ServiceLocator::GetRenderer()->AddMeshToScene(mesh);
	Ref<Revid::Mesh> planeMesh = MakeRef<Revid::Mesh>("./assets/obj/plane.obj");
	// The model matrix which is pretty big.
	planeMesh->SetInstanceCount(1);
	// Translate the plane to 500, 500.
	glm::mat4 modelMatrix = glm::translate(glm::mat4(100.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	planeMesh->SetModelMatrix(modelMatrix);
	Revid::ServiceLocator::GetRenderer()->AddMeshToScene(planeMesh);
	//Revid::ServiceLocator::GetRenderer()->UpdateObj("./assets/obj/bunny.obj");

	// Imgui setup
	// Create Framebuffers
	int w, h;
	GLFWwindow* window = Revid::ServiceLocator::GetWindow()->GetGLFWWindow();
	glfwGetFramebufferSize(window, &w, &h);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	Revid::ServiceLocator::GetRenderer()->CreateImguiDescriptorPool();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontDefault();
	io.Fonts->Build();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForVulkan(window, true);
	ImGui_ImplVulkan_InitInfo init_info = Revid::ServiceLocator::GetRenderer()->GetInitInfo();

	ImGui_ImplVulkan_Init(&init_info);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != nullptr);

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
}

void RevidEditor::Application::initializeLogger()
{
    // Logger::GetInstance().SetLogfile("logfile.txt");
}

void RevidEditor::Application::shutdownServices()
{
    Revid::ServiceLocator::ShutdownServices();
}

void RevidEditor::Application::setupImgui()
{
	// SetupImgui();
}

void RevidEditor::Application::updateImgui()
{
	// UpdateImgui();
}
