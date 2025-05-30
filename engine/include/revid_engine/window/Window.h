#pragma once
#define GLFW_INCLUDE_VULKAN
#include <maths/Vec.h>
#include <types/Containers.h>
#include <GLFW/glfw3.h>


namespace Revid
{
    struct WindowData
    {
        String m_title;
        uint32_t m_width, m_height;
    };

    enum class SurfaceArgs {
        INSTANCE,
        ALLOCATORS,
        OUT_SURFACE
    };

    class Window
    {
    public:
        GLFWwindow* GetGLFWWindow() const { return m_window; }
        std::pair<int, int> GetSize() const { return {m_windowData.m_width, m_windowData.m_height}; };
        uint32_t GetExtentionCount() const { return m_extensionCount; };
        const char** GetExtentions() const  { return m_extensions; };
        const WindowData& GetWindowData() const { return m_windowData; }

        Window() = default;
        void OpenWindow(const WindowData&);
        bool Update();
        void Shutdown();
        void GetDrawSurface(Map<SurfaceArgs, int*>);
        void WaitForEvents();
        bool IsKeyPressed(int key);
        bool IsMouseButtonPressed(int button);
        Maths::Vec2 GetMousePos();

    private:
        GLFWwindow* m_window;
        uint32_t m_extensionCount = 0;
        const char** m_extensions;
        WindowData m_windowData;

        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    };
}

