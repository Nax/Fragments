file(GLOB_RECURSE SOURCES "*.c" "*.asm")

add_library(libc STATIC ${SOURCES})
add_library(libc_kernel STATIC ${SOURCES})

target_include_directories(libc PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_include_directories(libc_kernel PUBLIC "${CMAKE_SOURCE_DIR}/include")

set_target_properties(libc PROPERTIES OUTPUT_NAME c)
set_target_properties(libc_kernel PROPERTIES OUTPUT_NAME c_kernel)

target_compile_options(libc PRIVATE ${COMPILE_OPTIONS_FREESTANDING})
target_compile_options(libc_kernel PRIVATE ${COMPILE_OPTIONS_KERNEL})
