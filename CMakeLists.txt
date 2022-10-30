project(SdlExamples)
cmake_minimum_required(VERSION 3.19)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON )
set(cpp_std 17)
find_package(SDL2 REQUIRED)
include_directories(${SDL_INCLUDE_DIRS})


option(USE_OS_SDL2_IMAGE "Use OS SDL2_image" OFF)

if (NOT USE_OS_SDL2_IMAGE)
	add_subdirectory(external/SDL_image)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/SDL_image)
endif()

add_subdirectory(src)
