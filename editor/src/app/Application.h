#pragma once
#include <imgui.h>
#include <types/Containers.h>

namespace RevidEditor
{
    class Application
    {
    private:
        String m_title = "revid";
        bool m_isRunning;
        ImVec4 m_clearColor;
        ImGuiIO m_imguiIo;

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
