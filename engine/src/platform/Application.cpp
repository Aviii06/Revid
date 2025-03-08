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
	system("pwd");
    Logger::Log(LogLevel::INFO, "Initialising Revid Application named: " + title);
    initializeLogger();
    intializeServices();

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

    	// TODO: Abstract this out to a InputHandler
		if (ServiceLocator::GetWindow()->IsKeyPressed(87))
		{
			ServiceLocator::GetCamera()->MoveForward();
		}
    	if (ServiceLocator::GetWindow()->IsKeyPressed(83))
    	{
			ServiceLocator::GetCamera()->MoveBackward();
    	}
    	if (ServiceLocator::GetWindow()->IsKeyPressed(65))
    	{
    		ServiceLocator::GetCamera()->MoveLeft();
    	}
    	if (ServiceLocator::GetWindow()->IsKeyPressed(68))
    	{
    		ServiceLocator::GetCamera()->MoveRight();
    	}
    	if (ServiceLocator::GetWindow()->IsKeyPressed('L'))
    	{
    		ServiceLocator::GetCamera()->MoveUp();
    	}
    	if (ServiceLocator::GetWindow()->IsKeyPressed('H'))
    	{
    		ServiceLocator::GetCamera()->MoveDown();
    	}

   //  	// Process Mouse Movements
   //  	if (ServiceLocator::GetWindow()->IsKeyPressed(341))
   //  	{
   //  		xoffset = ServiceLocator::GetWindow()->GetMouseX() - lastX;
			// ServiceLocator::GetCamera()->ProcessMouseMovement();
   //  	}

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
    system("pwd");
	Ref<Mesh> mesh = MakeRef<Mesh>("./assets/obj/grass.obj");
	mesh->SetInstanceCount(10000);
	ServiceLocator::GetRenderer()->AddMeshToScene(mesh);
	Ref<Mesh> planeMesh = MakeRef<Mesh>("./assets/obj/plane.obj");
	// The model matrix which is pretty big.
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -10.0f, 0.0f)) * glm::mat4(10.0f);
	planeMesh->SetModelMatrix(modelMatrix);
	planeMesh->SetInstanceCount(100);
	ServiceLocator::GetRenderer()->AddMeshToScene(planeMesh);
}

void Revid::Application::initializeLogger()
{
    // Logger::GetInstance().SetLogfile("logfile.txt");
}

void Revid::Application::shutdownServices()
{
    ServiceLocator::ShutdownServices();
}
