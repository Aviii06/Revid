add_subdirectory(glm)
add_subdirectory(glfw)

add_library(vendor INTERFACE)
find_package(Vulkan REQUIRED)

set(IMGUI_DIR imgui)

set(IMGUI_SOURCES
  ${IMGUI_DIR}/imgui.cpp
  # ${IMGUI_DIR}/imgui_demo.cpp # optional
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  # GLFW + Vulkan
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
  ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
)

# build imgui lib
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(
    imgui
  PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${IMGUI_DIR}/misc/cpp
)
target_link_libraries(
    imgui
  PUBLIC
    glfw
    Vulkan::Vulkan
)

target_link_libraries(vendor
        INTERFACE
            glfw
            glm
            Vulkan::Headers
            Vulkan::Vulkan
            imgui)
