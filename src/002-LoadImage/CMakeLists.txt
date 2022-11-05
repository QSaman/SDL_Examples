set(project_name loadImage)
project(${project_name})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
file(GLOB_RECURSE SRC_LIST src/*.c* src/*.h*)

add_executable(${project_name} ${SRC_LIST})

if (USE_OS_SDL2_IMAGE)

	# find_package(SDL2_image REQUIRED) doesn't work, so we use FindPkgConfig
	INCLUDE(FindPkgConfig)
	PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
	include_directories(${SDL2IMAGE_INCLUDE_DIRS})
	target_link_libraries(${project_name} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
else()
	target_link_libraries(${project_name} ${SDL2_LIBRARIES} SDL2_image)
endif()


set_property(TARGET ${project_name} PROPERTY CXX_STANDARD ${cpp_std})
set_property(TARGET ${project_name} PROPERTY CXX_STANDARD_REQUIRED ON)

add_custom_command(
	TARGET ${project_name} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_CURRENT_SOURCE_DIR}/resources/anakin013.jpg
	${CMAKE_CURRENT_BINARY_DIR}/bin)
