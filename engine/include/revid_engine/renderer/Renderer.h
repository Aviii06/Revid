#pragma once
#include <types/Containers.h>

namespace Revid
{
    struct RendererSettings
    {
        String appName;
        const char** windowExtentions;
        uint32_t windowExtentionCount;
    };

    class Renderer
    {
    public:
        virtual void Init(RendererSettings) = 0;
        virtual void Shutdown() = 0;
        virtual void Render() = 0;
    };
}
