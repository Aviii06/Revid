#pragma once
#define GLFW_INCLUDE_VULKAN
#include <revid_engine/platform/Window.h>
#include <types/SmartPointers.h>
#include <GLFW/glfw3.h>


namespace Revid
{
    class CustomWindow : public Window
    {
    private:
        GLFWwindow* m_window;

    public:
        CustomWindow() = default;
        void OpenWindow(WindowData) override;
        bool Update() override;
        void Shutdown() override;
        void GetDrawSurface(Map<SurfaceArgs, int*>) override;
        std::pair<int, int> GetSize() override;
    };
}

