#pragma once
#include <types/Containers.h>
#include <cstdint>

namespace Revid
{
    struct SimpleVertex;
    struct RendererSettings
    {
        uint8_t MAX_FRAMES_IN_FLIGHT;
        String appName;
        const char** windowExtentions;
        uint32_t windowExtentionCount;
    };

    class Renderer
    {
    public:
        virtual void Init(const RendererSettings&) = 0;
        virtual void Shutdown() = 0;
        virtual void Render() = 0;
        void FramebufferResized() { m_framebufferResized = true; }
        virtual void UpdateObj(String path) = 0;
        virtual void UpdateVertices(Vector<SimpleVertex>) = 0;
        virtual void UpdateIndices(Vector<uint16_t>) = 0;

    protected:
        bool m_framebufferResized = false;
    };
}
