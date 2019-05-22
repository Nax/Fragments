file(GLOB_RECURSE SOURCES "*.asm" "*.c")
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/stage2.ld")

add_executable(boot_stage2 ${SOURCES})
set_target_properties(boot_stage2 PROPERTIES LINKER_LANGUAGE C LINK_DEPENDS "${LINKER_SCRIPT}")
target_compile_options(
    boot_stage2
    PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-ffreestanding>
    $<$<COMPILE_LANGUAGE:C>:-nostdlib>
    $<$<COMPILE_LANGUAGE:C>:-mno-sse>
    $<$<COMPILE_LANGUAGE:C>:-mno-sse2>
    $<$<COMPILE_LANGUAGE:C>:-mno-red-zone>
)
target_link_options(boot_stage2 PRIVATE -ffreestanding -nostdlib -mno-sse -mno-sse2 -mno-red-zone -lgcc -T "${LINKER_SCRIPT}")
