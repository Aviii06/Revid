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
        static inline Ptr<EntityManager> s_entityManager = nullptr;
        static inline Ptr<ComponentManager> s_componentManager = nullptr;
        static inline Ptr<SystemManager> s_systemManager = nullptr;

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
        static inline void Provide(EntityManager* entityManager)
        {
            if (s_entityManager != nullptr) return;
            s_entityManager = std::unique_ptr<EntityManager>(entityManager);
        }

        static inline void Provide(ComponentManager* componentManager)
        {
            if (s_componentManager != nullptr) return;
            s_componentManager = std::unique_ptr<ComponentManager>(componentManager);
        }

        static inline void Provide(SystemManager* systemManager)
        {
            if (s_systemManager != nullptr) return;
            s_systemManager = std::unique_ptr<SystemManager>(systemManager);
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

                    static_assert(std::is_same_v<decltype(Systems::GetSignature()), Signature>,
                                  "System must define: static Signature GetSignature()");

                    // Create and register the system
                    const char* systemName = typeid(Systems).name();
                    GetSystemManager()->RegisterSystem<Systems>(systemName);;
                    GetSystemManager()->SetSignature<Systems>(Systems::GetSignature());
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
                    GetComponentManager()->RegisterComponent<Components>();;
                }(), 0)...
            };
        }


        static inline const Ptr<Window>& GetWindow() { return s_window; }
        static inline const Ptr<VulkanRenderer>& GetRenderer() { return s_renderer; }
        static inline const Ptr<EditorCamera>& GetCamera() { return s_camera; }
        static inline const Ptr<InputHandler>& GetInputHandler() { return s_inputHandler; }

        static inline const Ptr<EntityManager>& GetEntityManager() { return s_entityManager; }
        static inline const Ptr<ComponentManager>& GetComponentManager() { return s_componentManager; }
        static inline const Ptr<SystemManager>& GetSystemManager() { return s_systemManager; }

        template <typename T>
        static inline const Ref<T> GetSystem()
        {
            return GetSystemManager()->GetSystem<T>();
        }

        static inline void ShutdownServices()
        {
            shutdownWindow();
            shutdownRenderer();
        }
    };
}

