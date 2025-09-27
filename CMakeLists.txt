cmake_minimum_required(VERSION 3.15)




project(2D-shooter LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/std:c++latest)
endif()

include(cmake/CPM.cmake)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

CPMAddPackage("gh:RobLoach/raylib-cpp@5.5.0")
CPMAddPackage("gh:scastd/raygui-cpp#main")
CPMAddPackage("gh:raysan5/raygui#4.0")
CPMAddPackage("gh:erincatto/box2d@3.1.1")
CPMAddPackage("gh:zpl-c/enet@2.6.5")
CPMAddPackage("gh:nlohmann/json@3.12.0")
CPMAddPackage("gh:jupyter-xeus/cpp-terminal@0.1.0")
CPMAddPackage("gh:EclipseMenu/discord-presence#main")

add_subdirectory(src/core)
add_subdirectory(src/app)
add_subdirectory(src/server)