#include "CustomWindow.h"

void CustomWindow::OpenWindow()
{
    glfwInit();
    m_window = glfwCreateWindow(1920, 1080, "Hello World", nullptr, nullptr);
}

bool CustomWindow::Update()
{
    glfwPollEvents();

    return glfwWindowShouldClose(m_window);
}
