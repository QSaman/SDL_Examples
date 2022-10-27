project(SdlExamples)
cmake_minimum_required(VERSION 3.19)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON )
set(cpp_std 17)
find_package(SDL2 REQUIRED)
include_directories(${SDL_INCLUDE_DIRS})
add_subdirectory(src)
