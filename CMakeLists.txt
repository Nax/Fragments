cmake_minimum_required(VERSION 3.13)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

option(SUPERBUILD "Build this project as a superbuild" ON)
option(UTILS "Build the utils" OFF)

if (SUPERBUILD)
    project(SUPERBUILD NONE)

    find_package(Bochs)

    include(ExternalProject)

    ExternalProject_Add(
        FragmentsUtils
        SOURCE_DIR      "${CMAKE_SOURCE_DIR}"
        INSTALL_DIR     "${CMAKE_BINARY_DIR}/prefix"
        CMAKE_ARGS
            "-DSUPERBUILD=OFF"
            "-DUTILS=ON"
            "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
            "-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
    )
    ExternalProject_Add_Step(
		FragmentsUtils
		reconfigure
		COMMAND ${CMAKE_COMMAND} -E echo "[FragmentsUtils] Forcing a superbuild reconfigure"
		DEPENDEES download
		DEPENDERS configure
		ALWAYS ON
    )

    ExternalProject_Add(
        Fragments
        SOURCE_DIR      "${CMAKE_SOURCE_DIR}"
        INSTALL_DIR     "${CMAKE_BINARY_DIR}/fragments"
        CMAKE_ARGS
            "-DSUPERBUILD=OFF"
            "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_SOURCE_DIR}/cmake/CrossToolchain.cmake"
            "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
            "-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
            "-DCMAKE_PREFIX_PATH=${CMAKE_BINARY_DIR}/prefix"
        DEPENDS
            FragmentsUtils
    )
    ExternalProject_Add_Step(
		Fragments
		reconfigure
		COMMAND ${CMAKE_COMMAND} -E echo "[Fragments] Forcing a superbuild reconfigure"
		DEPENDEES download
		DEPENDERS configure
		ALWAYS ON
    )

    add_custom_target(
        run
        COMMAND
        Bochs::bochs -q -f "${CMAKE_SOURCE_DIR}/bochsrc.bxrc"
        WORKING_DIRECTORY
        "${CMAKE_BINARY_DIR}/fragments"
        VERBATIM
    )
else()
    if (UTILS)
        project(FragmentsUtils C CXX)
        add_subdirectory(util)
    else()
        project(Fragments C ASM_NASM)
        # We will handle the output format of NASM ourselves
        set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> <FLAGS> -o <OBJECT> <SOURCE>")
        add_subdirectory(src)
    endif()
endif()
