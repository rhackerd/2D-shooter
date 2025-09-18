cmake_minimum_required(VERSION 3.15)

project(2D-shooter LANGUAGES CXX)

include(cmake/CPM.cmake)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

CPMAddPackage("gh:RobLoach/raylib-cpp@5.5.0")
CPMAddPackage("gh:scastd/raygui-cpp#main")
CPMAddPackage("gh:raysan5/raygui#4.0")

add_subdirectory(src/core)
add_subdirectory(src/app)