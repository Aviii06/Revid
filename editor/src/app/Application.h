#pragma once
#include <types/Containers.h>

namespace Revid
{
    class Application
    {
    private:
        String m_title;
        bool m_isRunning;

        void initializeLogger();
        void intializeServices();
        void shutdownServices();

    protected:
        virtual void Update(float deltaTime) {};

    public:
        Application();
        explicit Application(String title);

        ~Application();

        void Run();
    };

}

extern Revid::Application* CreateApplication();

