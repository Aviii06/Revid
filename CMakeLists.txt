cmake_minimum_required(VERSION 3.28)
project(revid VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

add_subdirectory(vendor)
add_subdirectory(engine)
add_subdirectory(editor)
