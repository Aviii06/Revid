cmake_minimum_required(VERSION 3.5)
project(revid_engine VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        src/platform/CustomWindow.cpp
        src/Engine.cpp
        src/platform/Application.cpp
        src/renderer/vulkan/VulkanRenderer.cpp)
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