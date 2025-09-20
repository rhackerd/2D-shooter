file(GLOB_RECURSE CORE_SRC "*.cpp")

add_library(Core STATIC ${CORE_SRC})

target_include_directories(Core PUBLIC include)


target_link_libraries(Core
    PUBLIC
        raylib_cpp
        raygui_cpp
        raylib
)

target_include_directories(Core PUBLIC ${raygui_SOURCE_DIR}/src ${Physac_SOURCE_DIR}/src)