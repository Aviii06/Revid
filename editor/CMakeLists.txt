cmake_minimum_required(VERSION 3.5)

project(revid_app VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
        src/app/Application.cpp
        src/main.cpp)

target_link_libraries(${PROJECT_NAME}
        revid_engine
        vendor)
