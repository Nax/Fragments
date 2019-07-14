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

file(GLOB_RECURSE SOURCES "*.c" "*.asm")

add_library(libc STATIC ${SOURCES})
add_library(libc_kernel STATIC ${SOURCES})

target_include_directories(libc PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_include_directories(libc_kernel PUBLIC "${CMAKE_SOURCE_DIR}/include")

set_target_properties(libc PROPERTIES OUTPUT_NAME c)
set_target_properties(libc_kernel PROPERTIES OUTPUT_NAME c_kernel)

target_compile_options(libc PRIVATE ${COMPILE_OPTIONS_FREESTANDING})
target_compile_options(libc_kernel PRIVATE ${COMPILE_OPTIONS_KERNEL})
target_compile_definitions(libc_kernel PRIVATE __KERNEL__=1)
