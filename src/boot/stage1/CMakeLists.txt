file(GLOB_RECURSE SOURCES "*.asm")
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/stage1.ld")

add_executable(boot_stage1 ${SOURCES})
set_target_properties(boot_stage1 PROPERTIES LINKER_LANGUAGE C LINK_DEPENDS "${LINKER_SCRIPT}")
target_link_options(boot_stage1 PRIVATE -ffreestanding -nostdlib -T "${LINKER_SCRIPT}")
