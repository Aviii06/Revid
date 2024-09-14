#include "revid_engine/platform/Application.h"
#include "revid_engine/ServiceLocater.h"
#include "CustomWindow.h"
#include "renderer/vulkan/VulkanRenderer.h"
#include "logging/Logging.h"

Revid::Application::Application()
    : Application("Revid")
{
}

Revid::Application::Application(String title)
    : m_title(title), m_isRunning(true)
{
    Logger::Log(LogLevel::INFO, "Initialising Revid Application named: " + title);
    initializeLogger();
    intializeServices();
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

    RendererSettings settings {
    	.MAX_FRAMES_IN_FLIGHT = 3,
        .appName =  m_title,
        .windowExtentions = ServiceLocator::GetWindow()->GetExtentions(),
        .windowExtentionCount = ServiceLocator::GetWindow()->GetExtentionCount()
    };

    ServiceLocator::Provide(new VulkanRenderer(), settings);
}

void Revid::Application::initializeLogger()
{
    // Logger::GetInstance().SetLogfile("logfile.txt");
}

void Revid::Application::shutdownServices()
{
    ServiceLocator::ShutdownServices();
}
