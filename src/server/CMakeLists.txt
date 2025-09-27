file(GLOB_RECURSE SERVER_SRC "*.cpp" "*.c")

add_library(Server STATIC ${SERVER_SRC})

target_include_directories(Server PUBLIC include)


target_link_libraries(Server
    PUBLIC
        box2d
        enet_static
        cpp-terminal
)

target_include_directories(Server PUBLIC ${raygui_SOURCE_DIR}/src)