include(FindPackageHandleStandardArgs)

if (NOT MKIMAGE_FOUND)
    find_program(MKIMAGE_PATH mkimage)
    add_executable(FragmentsUtils::mkimage IMPORTED)
    set_target_properties(FragmentsUtils::mkimage PROPERTIES IMPORTED_LOCATION "${MKIMAGE_PATH}")

    find_package_handle_standard_args(mkimage DEFAULT_MSG MKIMAGE_PATH)
endif()
