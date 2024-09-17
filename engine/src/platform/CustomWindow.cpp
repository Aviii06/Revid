#include "CustomWindow.h"
#include <exceptions/RevidRuntimeException.h>
#include <revid_engine/ServiceLocater.h>

void Revid::CustomWindow::OpenWindow(const WindowData& windowData)
{
	m_windowData.m_title = std::move(windowData.m_title);
	m_windowData.m_height = windowData.m_height;
	m_windowData.m_width = windowData.m_width;
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    m_window = glfwCreateWindow(
        windowData.m_width,
        windowData.m_height,
        windowData.m_title.c_str(), nullptr, nullptr);

    m_extensions = glfwGetRequiredInstanceExtensions(&m_extensionCount);

    glfwSetWindowUserPointer(m_window, this);
	glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
}

bool Revid::CustomWindow::Update()
{
    glfwPollEvents();

    return glfwWindowShouldClose(m_window);
}

void Revid::CustomWindow::Shutdown()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool Revid::CustomWindow::IsKeyPressed(int key)
{
	int state = glfwGetKey(m_window, key);

	return state == GLFW_PRESS || state == GLFW_REPEAT;
}


void Revid::CustomWindow::GetDrawSurface(Map<SurfaceArgs, int*> surfaceArgs)
{
    auto vkInstance = reinterpret_cast<VkInstance>(surfaceArgs[SurfaceArgs::INSTANCE]);
    auto *allocationCallbacks = surfaceArgs[SurfaceArgs::ALLOCATORS] ?
            reinterpret_cast<VkAllocationCallbacks *>(surfaceArgs[SurfaceArgs::ALLOCATORS]): nullptr;
    auto *outSurface = reinterpret_cast<VkSurfaceKHR*>(surfaceArgs[SurfaceArgs::OUT_SURFACE]);

    if (vkInstance == VK_NULL_HANDLE)
    {
        throw RevidRuntimeException("Must provide an instance!");
    }


    if (glfwCreateWindowSurface(vkInstance, m_window, allocationCallbacks, outSurface) != VK_SUCCESS)
    {
        throw RevidRuntimeException("GLFW surface creation error");
    }
}

// std::pair<int, int> Revid::CustomWindow::GetSize()
// {
// 	int width, height;
// 	glfwGetFramebufferSize(m_window, &width, &height);
//
// 	return {width, height};
// }

void Revid::CustomWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto m_window = reinterpret_cast<CustomWindow*>(glfwGetWindowUserPointer(window));
	m_window->m_windowData.m_width = width;
	m_window->m_windowData.m_height = height;
	ServiceLocator::GetRenderer()->FramebufferResized();
}

void Revid::CustomWindow::WaitForEvents()
{
	glfwWaitEvents();
}

