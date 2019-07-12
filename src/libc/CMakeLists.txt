file(GLOB_RECURSE SOURCES "*.c")

add_library(libc STATIC ${SOURCES})
add_library(libc_kernel STATIC ${SOURCES})

target_include_directories(libc PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_include_directories(libc_kernel PUBLIC "${CMAKE_SOURCE_DIR}/include")

set_target_properties(libc PROPERTIES OUTPUT_NAME c)
set_target_properties(libc_kernel PROPERTIES OUTPUT_NAME c_kernel)

target_compile_options(libc PRIVATE ${COMPILE_OPTIONS_FREESTANDING})
target_compile_options(libc_kernel PRIVATE ${COMPILE_OPTIONS_KERNEL})

set(CRT0 "${CMAKE_BINARY_DIR}/crt0.o" CACHE INTERNAL "")
set(CRTI "${CMAKE_BINARY_DIR}/crti.o" CACHE INTERNAL "")
set(CRTN "${CMAKE_BINARY_DIR}/crtn.o" CACHE INTERNAL "")

macro(assemble_single _target _src _obj)
    add_custom_command(
        OUTPUT      "${_obj}"
        DEPENDS     "${_src}"
        COMMAND     "${CMAKE_ASM_NASM_COMPILER}" -f elf64 "${_src}" -o "${_obj}"
        COMMENT     "Assembling ${_target}.o"
        VERBATIM
    )
    add_custom_target(
        ${_target}
        DEPENDS ${_obj}
        VERBATIM
    )
    add_dependencies(libc ${_target})
endmacro()

assemble_single(crt0 "${CMAKE_CURRENT_SOURCE_DIR}/arch/x86_64/crt0.asm" "${CRT0}")
assemble_single(crti "${CMAKE_CURRENT_SOURCE_DIR}/arch/x86_64/crti.asm" "${CRTI}")
assemble_single(crtn "${CMAKE_CURRENT_SOURCE_DIR}/arch/x86_64/crtn.asm" "${CRTN}")
