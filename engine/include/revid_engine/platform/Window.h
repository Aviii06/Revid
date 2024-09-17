#pragma once
#include <types/Containers.h>
#include <any>

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
    protected:
        uint32_t m_extensionCount = 0;
        const char** m_extensions;
        WindowData m_windowData;

    public:
        virtual void OpenWindow(const WindowData&) = 0;
        virtual bool Update() = 0;
        virtual void Shutdown() = 0;
        [[nodiscard]] std::pair<int, int> GetSize() const { return {m_windowData.m_width, m_windowData.m_height}; };
        [[nodiscard]] uint32_t GetExtentionCount() const { return m_extensionCount; };
        [[nodiscard]] const char** GetExtentions() const  { return m_extensions; };
        [[nodiscard]] const WindowData& GetWindowData() const { return m_windowData; }

        virtual void WaitForEvents() = 0;

        virtual void GetDrawSurface(Map<SurfaceArgs, int*>) = 0;

        virtual bool IsKeyPressed(int key) = 0;
    };
}
