#include "revid_engine/platform/Application.h"
#include "revid_engine/ServiceLocater.h"
#include "CustomWindow.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/transform.hpp"
#include "revid_engine/renderer/vulkan/VulkanRenderer.h"
#include "logging/Logging.h"

Revid::Application::Application()
    : Application("Revid")
{
}

Revid::Application::Application(String title)
    : m_title(std::move(title)), m_isRunning(true)
{
    Logger::Log(LogLevel::INFO, "Initialising Revid Application named: " + title);
    initializeLogger();
    intializeServices();


	Ref<Mesh> mesh = MakeRef<Mesh>("./assets/obj/grass.obj");
	mesh->SetInstanceCount(10000);
	glm::mat4 modelMatrix2 = glm::mat4(1.0f);
	mesh->SetModelMatrix(modelMatrix2);
	ServiceLocator::GetRenderer()->AddMeshToScene(mesh);
	Ref<Mesh> planeMesh = MakeRef<Mesh>("./assets/obj/plane.obj");
	// The model matrix which is pretty big.
	planeMesh->SetInstanceCount(1);
	// Translate the plane to 500, 500.
	glm::mat4 modelMatrix = glm::mat4(10.0f);
	planeMesh->SetModelMatrix(modelMatrix);
	// ServiceLocator::GetRenderer()->AddMeshToScene(planeMesh);
	//ServiceLocator::GetRenderer()->UpdateObj("./assets/obj/bunny.obj");
}

Revid::Application::~Application()
{
    shutdownServices();
}

void Revid::Application::Run()
{
    while(m_isRunning)
    {
        if (ServiceLocator::GetWindow()->Update())
        {
            m_isRunning = false;
            continue;
        }

        Update(0.01f);

    	ServiceLocator::GetInputHandler()->HandleInput();

        ServiceLocator::GetRenderer()->Render();
    }
}

void Revid::Application::intializeServices()
{
    ServiceLocator::Provide(new CustomWindow());

    ServiceLocator::GetWindow()->OpenWindow({
        m_title,
        1920,
        1080
    });

	ServiceLocator::Provide(new EditorCamera());

    RendererSettings settings {
    	3,
          m_title,
        ServiceLocator::GetWindow()->GetExtentions(),
        ServiceLocator::GetWindow()->GetExtentionCount()
    };

    ServiceLocator::Provide(new VulkanRenderer(), settings);

	ServiceLocator::Provide(new InputHandler());
}

void Revid::Application::initializeLogger()
{
    // Logger::GetInstance().SetLogfile("logfile.txt");
}

void Revid::Application::shutdownServices()
{
    ServiceLocator::ShutdownServices();
}
