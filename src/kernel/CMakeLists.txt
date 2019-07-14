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

file(GLOB_RECURSE SOURCES "*.asm" "*.c")

set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld")
add_executable(kernel ${SOURCES})
set_target_properties(kernel PROPERTIES LINKER_LANGUAGE C LINK_DEPENDS "${LINKER_SCRIPT}")
target_compile_options(
    kernel
    PRIVATE
    $<$<COMPILE_LANGUAGE:ASM_NASM>: -f elf64>
    $<$<COMPILE_LANGUAGE:C>: -ffreestanding>
    $<$<COMPILE_LANGUAGE:C>: -nostdlib>
    $<$<COMPILE_LANGUAGE:C>: -mno-red-zone>
    $<$<COMPILE_LANGUAGE:C>: -mcmodel=kernel>
)
target_include_directories(kernel PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_link_options(kernel PRIVATE -ffreestanding -nostdlib -mcmodel=kernel -z max-page-size=0x1000 -T "${LINKER_SCRIPT}")
target_link_libraries(kernel libc_kernel)
