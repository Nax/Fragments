#
# Copyright (c) 2019, Maxime Bacoux
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

cmake_minimum_required(VERSION 3.13)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

option(SUPERBUILD "Build this project as a superbuild" ON)
option(UTILS "Build the utils" OFF)

if (SUPERBUILD)
    project(SUPERBUILD NONE)

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
