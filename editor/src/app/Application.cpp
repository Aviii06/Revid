#include "Application.h"
#include "revid_engine/ServiceLocater.h"
#include <revid_engine/window/CustomWindow.h>
#include <revid_engine/core/renderer/VulkanRenderer.h>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/transform.hpp"
#include "logging/Logging.h"
#include "gui/ImguiHelper.h"

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

        Update(0.01f);

    	Revid::ServiceLocator::GetInputHandler()->HandleInput();
		updateImgui();

        Revid::ServiceLocator::GetRenderer()->Render();
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
	setupImgui();
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
	SetupImgui();
}

void RevidEditor::Application::updateImgui()
{
	UpdateImgui();
}
