cmake_minimum_required(VERSION 3.10)

option(SUPERBUILD "Build this project as a superbuild" ON)

if (SUPERBUILD)
    project(SUPERBUILD NONE)

    include(ExternalProject)
    ExternalProject_Add(
        Fragments
        SOURCE_DIR      "${CMAKE_SOURCE_DIR}"
        INSTALL_DIR     "${CMAKE_BINARY_DIR}"
        CMAKE_ARGS
            "-DSUPERBUILD=OFF"
            "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_SOURCE_DIR}/cmake/CrossToolchain.cmake"
            "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
            "-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
    )
    ExternalProject_Add_Step(
		Fragments
		reconfigure
		COMMAND ${CMAKE_COMMAND} -E echo "Forcing a superbuild reconfigure"
		DEPENDEES download
		DEPENDERS configure
		ALWAYS ON
    )
else()
    project(Fragments)
    add_subdirectory(src)
endif()
