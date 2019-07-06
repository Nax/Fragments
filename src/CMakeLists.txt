find_package(FragmentsUtils REQUIRED)

add_subdirectory(boot)
add_subdirectory(kernel)
add_subdirectory(libc)

install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/" DESTINATION include)

add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/fragments.img"
    COMMAND
        FragmentsUtils::mkimage
        "${CMAKE_BINARY_DIR}/fragments.img"
        $<TARGET_FILE:boot_stage1>
        $<TARGET_FILE:boot_stage2>
        $<TARGET_FILE:kernel>
    DEPENDS
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
