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
    public:
        virtual void OpenWindow(WindowData) = 0;
        virtual bool Update() = 0;
        virtual void Shutdown() = 0;
        virtual std::pair<int, int> GetSize() = 0;
        [[nodiscard]] uint32_t GetExtentionCount() const { return m_extensionCount; };
        [[nodiscard]] const char** GetExtentions() const  { return m_extensions; };

        virtual void GetDrawSurface(Map<SurfaceArgs, int*>) = 0;
    };
}
