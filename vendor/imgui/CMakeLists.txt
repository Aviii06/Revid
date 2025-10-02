add_library(imgui STATIC)

target_include_directories(imgui PUBLIC .)
target_include_directories(imgui PUBLIC backends)

target_sources(imgui PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui_demo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui_widgets.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
)

target_link_libraries(imgui PUBLIC Vulkan::Vulkan glfw)

add_executable(imgui_test example/main.cpp)
target_link_libraries(imgui_test imgui)
