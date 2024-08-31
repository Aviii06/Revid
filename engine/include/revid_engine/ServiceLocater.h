#pragma once
#include <types/SmartPointers.h>
#include "revid_engine/platform/Window.h"
#include "revid_engine/renderer/Renderer.h"
#include <logging/Logging.h>

//TODO: Use a singelton mixin to remove duplication
namespace Revid
{
    class ServiceLocator
    {
    private:
        static inline Ptr<Window> s_window = nullptr;
        static inline Ptr<Renderer> s_renderer = nullptr;

        static inline void shutdownWindow()
        {
            Logger::GetInstance().Log(LogLevel::INFO, "Shutting Down Window");
            s_window->Shutdown();
            s_window.reset();
            s_window = nullptr;
        }

        static inline void shutdownRenderer()
        {
            Logger::GetInstance().Log(LogLevel::INFO, "Shutting Down Renderer");
            s_renderer->Shutdown();
            s_renderer.reset();
        }

    public:
        static inline void Provide(Window *window)
        {
            Logger::GetInstance().Log(LogLevel::INFO, "Creating a Window");
            if (s_window != nullptr) return;
            s_window = std::unique_ptr<Window>(window);
        }

        static inline void Provide(Renderer *renderer, RendererSettings settings)
        {
            Logger::GetInstance().Log(LogLevel::INFO, "Creating Renderer");
            if (s_renderer != nullptr) return;

            s_renderer = std::unique_ptr<Renderer>(renderer);
            s_renderer->Init(settings);
        }

        static inline const Ptr<Window>& GetWindow() { return s_window; }
        static inline const Ptr<Renderer>& GetRenderer() { return s_renderer; }

        static inline void ShutdownServices()
        {
            shutdownWindow();
            shutdownRenderer();
        }
    };
}

