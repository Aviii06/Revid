#include "CustomWindow.h"
#include <exceptions/RevidRuntimeException.h>

void Revid::CustomWindow::OpenWindow(WindowData windowData)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_window = glfwCreateWindow(
        windowData.m_width,
        windowData.m_height,
        windowData.m_title.c_str(), nullptr, nullptr);

    m_extensions = glfwGetRequiredInstanceExtensions(&m_extensionCount);

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

std::pair<int, int> Revid::CustomWindow::GetSize()
{
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);

	return {width, height};
}



