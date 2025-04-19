cmake_minimum_required(VERSION 3.5)
project(revid_engine VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        src/Engine.cpp

        src/core/renderer/VulkanRenderer.cpp
        src/core/renderer/Mesh.cpp

        src/window/CustomWindow.cpp
        src/input/Camera.cpp
        src/input/InputHandler.cpp)

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:common>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
        vendor
)

file (GLOB SHADERS assets/shaders/*.frag assets/shaders/*.vert)
file (GLOB OBJS assets/obj/*.obj)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/editor/assets
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/editor/assets/shaders
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/editor/assets/obj)


foreach(shader ${SHADERS})
    get_filename_component(SHADER_NAME ${shader} NAME)

    add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
            COMMAND glslc -c ${shader} -o ${CMAKE_BINARY_DIR}/editor/assets/shaders/${SHADER_NAME}.spv)
endforeach()

file (COPY ${OBJS} DESTINATION ${CMAKE_BINARY_DIR}/editor/assets/obj/)
