#include "Application.h"
#include "revid_engine/ServiceLocator.h"
#include <revid_engine/window/Window.h>
#include <revid_engine/core/renderer/Renderer.h>
#include <revid_engine/core/ecs/systems/TransformSystem.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <revid_engine/core/ecs/components/model/ModelComponent.h>

#include "glm/gtx/transform.hpp"
#include "logging/Logging.h"
#include "gui/EditorUIManager.h"

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
    Revid::ServiceLocator::Provide(new Revid::Window());

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

	Revid::ServiceLocator::Provide(new Revid::ECSRegistry());
	Revid::ServiceLocator::InitialiseComponents<Revid::TransformComponent, Revid::ModelComponent>();
	Revid::ServiceLocator::InitialiseSystems<Revid::TransformSystem>();




	Revid::Entity grass = Revid::ServiceLocator::GetECSRegistry()->CreateEntity();
	Revid::TransformComponent tc = Revid::TransformComponent(Revid::Maths::Vec3(0.0f, 0.0f, 0.0f), Revid::Maths::Vec3(0.0f, 0.0f, 0.0f), Revid::Maths::Vec3(1.0f, 1.0f, 1.0f));
	Revid::ModelComponent mc = Revid::ModelComponent();
	Ref<Revid::Mesh> mesh = MakeRef<Revid::Mesh>("./assets/obj/grass.obj");
	mesh->SetInstanceCount(2e4);
	mc.m_meshes.push_back(mesh);
	Revid::ServiceLocator::GetECSRegistry()->AddComponent(grass, tc);
	Revid::ServiceLocator::GetECSRegistry()->AddComponent(grass, mc);

	// Revid::ServiceLocator::GetRenderer()->AddMeshToScene(mesh);


	Revid::Entity plane = Revid::ServiceLocator::GetECSRegistry()->CreateEntity();
	Revid::TransformComponent ptc = Revid::TransformComponent(Revid::Maths::Vec3(0.0f, 0.0f, 0.0f), Revid::Maths::Vec3(0.0f, 0.0f, 0.0f), Revid::Maths::Vec3(10.0f, 10.0f, 10.0f));
	Revid::ModelComponent pmc = Revid::ModelComponent();
	Revid::ServiceLocator::GetECSRegistry()->AddComponent(plane, ptc);
	Ref<Revid::Mesh> planeMesh = MakeRef<Revid::Mesh>("./assets/obj/plane.obj");
	planeMesh->SetInstanceCount(1);
	pmc.m_meshes.push_back(planeMesh);
	Revid::ServiceLocator::GetECSRegistry()->AddComponent(plane, pmc);

	// Revid::ServiceLocator::GetRenderer()->AddMeshToScene(planeMesh);
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
