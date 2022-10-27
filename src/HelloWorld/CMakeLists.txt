project(HelloWorld)
set(project_name HelloWorld)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
file(GLOB_RECURSE SRC_LIST src/*.c* src/*.h*)

add_executable(${project_name} ${SRC_LIST})
target_link_libraries(${project_name} ${SDL2_LIBRARIES})

set_property(TARGET ${project_name} PROPERTY CXX_STANDARD ${cpp_std})
set_property(TARGET ${project_name} PROPERTY CXX_STANDARD_REQUIRED ON)
