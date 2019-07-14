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

find_package(FragmentsUtils REQUIRED)

add_subdirectory(boot)
add_subdirectory(kernel)
add_subdirectory(libc)
add_subdirectory(crt)
add_subdirectory(userland)

install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/" DESTINATION include)

add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/kimage"
    COMMAND
        FragmentsUtils::mkrdisk
        "${CMAKE_BINARY_DIR}/kimage"
        "sbin/vfsd"
        $<TARGET_FILE:vfsd>
    DEPENDS
        vfsd
        FragmentsUtils::mkrdisk
    VERBATIM
)

add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/fragments.img"
    COMMAND
        FragmentsUtils::mkimage
        "${CMAKE_BINARY_DIR}/fragments.img"
        $<TARGET_FILE:boot_stage1>
        $<TARGET_FILE:boot_stage2>
        $<TARGET_FILE:kernel>
        "${CMAKE_BINARY_DIR}/kimage"
    DEPENDS
        "${CMAKE_BINARY_DIR}/kimage"
        boot_stage1
        boot_stage2
        kernel
        FragmentsUtils::mkimage
    VERBATIM
)

add_custom_target(
    image ALL
    DEPENDS "${CMAKE_BINARY_DIR}/fragments.img"
)

install(FILES "${CMAKE_BINARY_DIR}/fragments.img" DESTINATION .)
