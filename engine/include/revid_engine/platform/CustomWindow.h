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

        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    public:
        CustomWindow() = default;
        void OpenWindow(const WindowData&) override;
        bool Update() override;
        void Shutdown() override;
        void GetDrawSurface(Map<SurfaceArgs, int*>) override;
        void WaitForEvents() override;
        bool IsKeyPressed(int key) override;
        bool IsMouseButtonPressed(int button) override;
        Maths::Vec2 GetMousePos() override;

        GLFWwindow* GetGLFWWindow() { return m_window; }
    };
}

