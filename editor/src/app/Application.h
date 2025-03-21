#pragma once
#include <types/Containers.h>

namespace RevidEditor
{
    class Application
    {
    private:
        String m_title;
        bool m_isRunning;

        void initializeLogger();
        void intializeServices();
        void shutdownServices();
        void setupImgui();
        void updateImgui();

    protected:
        virtual void Update(float deltaTime) {};

    public:
        Application();
        explicit Application(String title);

        ~Application();

        void Run();
    };

}
