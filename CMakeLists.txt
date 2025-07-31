cmake_minimum_required(VERSION 3.31)

include(cmake/GlobalDefines.cmake)

project(
	${G_PROJECT_NAME}
	LANGUAGES CXX
)

include(cmake/BuildConfig.cmake)

file(GLOB_RECURSE SOURCES "src/*.cpp" "include/*.h" "include/*.hpp")

add_executable(
	${G_PROJECT_NAME}
	main.cpp
	${SOURCES}
)

target_include_directories(${G_PROJECT_NAME} PRIVATE src)
target_include_directories(${G_PROJECT_NAME} PUBLIC include)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)

target_link_libraries(
	${G_PROJECT_NAME}
	PRIVATE
	OpenGL::GL
	glfw
	glad::glad
)

set(
	DIRS_TO_COPY
	shaders
#	Assets
)
install(
	DIRECTORY ${DIRS_TO_COPY}
	DESTINATION ${G_BUILD_FOLDER}
)

add_custom_command(
	TARGET ${G_PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR} --prefix ${CMAKE_BINARY_DIR}
	COMMENT "[${G_PROJECT_NAME}::POST BUILD] Installing."
)