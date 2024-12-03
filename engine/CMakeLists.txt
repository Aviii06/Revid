cmake_minimum_required(VERSION 3.5)
project(revid_engine VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        src/platform/CustomWindow.cpp
        src/Engine.cpp
        src/platform/Application.cpp
        include/revid_engine/renderer/vulkan/VulkanRenderer.cpp
        include/revid_engine/platform/Camera.cpp
        include/revid_engine/renderer/vulkan/VertexBuffer.cpp
        include/revid_engine/renderer/Mesh.cpp
        include/revid_engine/renderer/vulkan/Buffer.cpp)
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

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND mkdir -p ${CMAKE_BINARY_DIR}/editor/assets
        COMMAND mkdir -p ${CMAKE_BINARY_DIR}/editor/assets/shaders
        COMMAND mkdir -p ${CMAKE_BINARY_DIR}/editor/assets/obj)

foreach(shader ${SHADERS})
    get_filename_component(SHADER_NAME ${shader} NAME)

    add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
            COMMAND glslc -c ${shader} -o ${CMAKE_BINARY_DIR}/editor/assets/shaders/${SHADER_NAME}.spv)
endforeach()

#file (COPY SHADERS assets/obj/bunny.obj ${CMAKE_BINARY_DIR}/editor/assets/obj/bunny.obj)
