add_executable(vfsd vfsd.c)

set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/userland.ld")
set_target_properties(vfsd PROPERTIES LINKER_LANGUAGE C LINK_DEPENDS "${LINKER_SCRIPT}")
target_compile_options(
    vfsd
    PRIVATE
    ${COMPILE_OPTIONS_USERLAND}
)
target_include_directories(vfsd PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_link_options(vfsd PRIVATE -ffreestanding -nostdlib -z max-page-size=0x1000 -T "${LINKER_SCRIPT}")
target_link_libraries(vfsd ${CRT0} ${CRTI} ${CRTN} libc gcc)
