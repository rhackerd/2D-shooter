file(GLOB_RECURSE APP_SRC "*.cpp")

add_executable(app ${APP_SRC})
target_include_directories(app PUBLIC include)
target_link_libraries(app PRIVATE Core)