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

file(GLOB_RECURSE SOURCES_ASM "*.asm")
file(GLOB_RECURSE SOURCES_C "*.c")

set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/stage2.ld")

# We need to link together 16, 32 and 64 bit code.
# CMake will absolutely hate this, so we need to do that in multiple steps.
# NASM has no issue with mixing code, but gcc can only
# output 32 bit or 64 bit code in elf32 and elf64, respectively.
# Since all C code here is 32 bit, the solution is to make a static lib out
# of those files, then objcopy the archive to elf64.
add_library(boot_stage2_32 STATIC ${SOURCES_C})
target_compile_options(
    boot_stage2_32
    PRIVATE
    -m32
    -ffreestanding
    -nostdlib
    -mno-mmx
    -mno-sse
    -mno-sse2
)

# GCC and ar will emit a 32 bit archive, we export it into elf64.
# Note that the code inside is stull untouched and 32 bit.
add_custom_command(
    TARGET boot_stage2_32 POST_BUILD
    COMMAND x86_64-elf-objcopy $<TARGET_FILE:boot_stage2_32> -O elf64-x86-64
    VERBATIM
)

# NASM has no issue with mixing, so we can happily assemble everything.
add_executable(boot_stage2 ${SOURCES_ASM})
set_target_properties(boot_stage2 PROPERTIES LINKER_LANGUAGE C LINK_DEPENDS "${LINKER_SCRIPT}")
target_compile_options(
    boot_stage2
    PRIVATE
    -f elf64
)

target_include_directories(boot_stage2 PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_include_directories(boot_stage2_32 PUBLIC "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(boot_stage2 boot_stage2_32)
target_link_options(boot_stage2 PRIVATE -ffreestanding -nostdlib -T "${LINKER_SCRIPT}")
