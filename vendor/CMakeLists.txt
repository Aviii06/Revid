add_subdirectory(glm)
add_subdirectory(glfw)
add_subdirectory(Vulkan-Headers)

add_library(vendor INTERFACE)
find_package(Vulkan REQUIRED)
add_subdirectory(imgui)

target_link_libraries(vendor
        INTERFACE
            glfw
            glm
            Vulkan::Headers
            Vulkan::Vulkan
            imgui)