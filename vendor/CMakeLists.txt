add_subdirectory(glm)
add_subdirectory(glfw)

add_library(vendor INTERFACE)

target_link_libraries(vendor
        INTERFACE
            glfw
            glm)