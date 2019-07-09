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
