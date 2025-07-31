########################################################################
# This cmake file contains a bunch of variables for managing the build #
########################################################################

include_guard(GLOBAL)

########################################################################################################################

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


########################################################################################################################

# Makes installation more universal and straightforward.
set(
	CMAKE_INSTALL_PREFIX
	${CMAKE_BINARY_DIR}
	CACHE INTERNAL ""
)

########################################################################################################################

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${G_BUILD_FOLDER})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${G_BUILD_FOLDER})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${G_BUILD_FOLDER})

########################################################################################################################

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(BUILD_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	add_compile_definitions(BUILD_RELEASE)
endif()