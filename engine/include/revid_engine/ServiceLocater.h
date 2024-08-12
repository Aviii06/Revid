#pragma once
#include <types/SmartPointers.h>
#include "revid_engine/platform/Window.h"

//TODO: Use a singelton mixin to remove duplication
class ServiceLocator
{
private:
    static inline Ptr<Window> s_window = nullptr;

public:
    static inline void Provide(Window *window) {
        if (s_window != nullptr) return;
        s_window = std::unique_ptr<Window>(window);
    }

    static inline Window* GetWindow() { return s_window.get(); }
};
