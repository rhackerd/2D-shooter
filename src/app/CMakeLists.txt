file(GLOB_RECURSE APP_SRC "*.cpp")

add_executable(2D-shooter ${APP_SRC})
target_include_directories(2D-shooter PUBLIC include)
target_link_libraries(2D-shooter PRIVATE Core Server)