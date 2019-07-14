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
