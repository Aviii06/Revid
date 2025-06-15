#pragma once
#include <revid_engine/window/Window.h>
#include <revid_engine/input/Camera.h>
#include <revid_engine/input/InputHandler.h>
#include <revid_engine/core/renderer/Renderer.h>
#include <revid_engine/core/ecs/EntityManager.h>
#include <revid_engine/core/ecs/ComponentManager.h>
#include <revid_engine/core/ecs/SystemManager.h>
#include <revid_engine/core/ecs/Coordinator.h>

#include <logging/Logging.h>
#include <types/SmartPointers.h>

//TODO: Use a singelton mixin to remove duplication
namespace Revid
{
    class ServiceLocator
    {
    private:
        static inline Ptr<Window> s_window = nullptr;
        static inline Ptr<VulkanRenderer> s_renderer = nullptr;
        static inline Ptr<EditorCamera> s_camera = nullptr;
        static inline Ptr<InputHandler> s_inputHandler = nullptr;

        // ECS subsystem
        static inline Ptr<Coordinator> s_coordinator = nullptr;

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
        static inline void Provide(Window *window)
        {
            Logger::Log(LogLevel::INFO, "Creating a Window");
            if (s_window != nullptr) return;
            s_window = std::unique_ptr<Window>(window);
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

        // ECS subsystem
        static inline void Provide(Coordinator* coordinator)
        {
            if (s_coordinator != nullptr) return;
            s_coordinator = std::unique_ptr<Coordinator>(coordinator);
            s_coordinator->Init();
        }

        template<typename... Systems>
        static void InitialiseSystems()
        {
            (void)std::initializer_list<int>
            {
                ([
                ]
                {
                    static_assert(std::is_base_of_v<System, Systems>,
                                  "System must inherit from the class System.");

                    GetECSCoordinator()->RegisterSystem<Systems>();
                }(), 0)...
            };
        }

        template<typename... Components>
        static void InitialiseComponents()
        {
            (void)std::initializer_list<int>
            {
                ([
                ]
                {
                    GetECSCoordinator()->RegisterComponent<Components>();;
                }(), 0)...
            };
        }


        static inline const Ptr<Window>& GetWindow() { return s_window; }
        static inline const Ptr<VulkanRenderer>& GetRenderer() { return s_renderer; }
        static inline const Ptr<EditorCamera>& GetCamera() { return s_camera; }
        static inline const Ptr<InputHandler>& GetInputHandler() { return s_inputHandler; }

        static inline const Ptr<Coordinator>& GetECSCoordinator() { return s_coordinator; }

        static inline void ShutdownServices()
        {
            shutdownWindow();
            shutdownRenderer();
        }
    };
}

