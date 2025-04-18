#pragma once
#include <types/SmartPointers.h>
#include "revid_engine/platform/CustomWindow.h"
#include "revid_engine/renderer/Renderer.h"
#include "revid_engine/platform/Camera.h"
#include <logging/Logging.h>
#include "revid_engine/platform/InputHandler.h"

#include "revid_engine/renderer/vulkan/VulkanRenderer.h"

//TODO: Use a singelton mixin to remove duplication
namespace Revid
{
    class ServiceLocator
    {
    private:
        static inline Ptr<CustomWindow> s_window = nullptr;
        static inline Ptr<VulkanRenderer> s_renderer = nullptr;
        static inline Ptr<EditorCamera> s_camera = nullptr;
        static inline Ptr<InputHandler> s_inputHandler = nullptr;

        static inline void shutdownWindow()
        {
            Logger::Log(LogLevel::INFO, "Shutting Down Window");
            s_window->Shutdown();
            s_window.reset();
            s_window = nullptr;
        }

        static inline void shutdownRenderer()
        {
            Logger::Log(LogLevel::INFO, "Shutting Down Renderer");
            s_renderer->Shutdown();
            s_renderer.reset();
        }

    public:
        static inline void Provide(CustomWindow *window)
        {
            Logger::Log(LogLevel::INFO, "Creating a Window");
            if (s_window != nullptr) return;
            s_window = std::unique_ptr<CustomWindow>(window);
        }

        static inline void Provide(VulkanRenderer *renderer, RendererSettings settings)
        {
            Logger::Log(LogLevel::INFO, "Creating Renderer");
            if (s_renderer != nullptr) return;

            s_renderer = std::unique_ptr<VulkanRenderer>(renderer);
            s_renderer->Init(settings);
        }

        static inline void Provide(EditorCamera *camera)
        {
            Logger::Log(LogLevel::INFO, "Creating Editor Camera");
            if (s_camera != nullptr) return;

            s_camera = std::unique_ptr<EditorCamera>(camera);
        }

        static inline void Provide(InputHandler* handler)
        {
            Logger::Log(LogLevel::INFO, "Creating Input Handler");
            if (s_inputHandler != nullptr) return;

            s_inputHandler = std::unique_ptr<InputHandler>(handler);
        }

        static inline const Ptr<CustomWindow>& GetWindow() { return s_window; }
        static inline const Ptr<VulkanRenderer>& GetRenderer() { return s_renderer; }
        static inline const Ptr<EditorCamera>& GetCamera() { return s_camera; }
        static inline const Ptr<InputHandler>& GetInputHandler() { return s_inputHandler; }

        static inline void ShutdownServices()
        {
            shutdownWindow();
            shutdownRenderer();
        }
    };
}

