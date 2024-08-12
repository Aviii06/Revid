#pragma once
#include <revid_engine/platform/Window.h>
#include <types/SmartPointers.h>
#include <GLFW/glfw3.h>


class CustomWindow : public Window
{
private:
    GLFWwindow* m_window;

public:
    CustomWindow() = default;
    void OpenWindow() override;
    bool Update() override;
};
