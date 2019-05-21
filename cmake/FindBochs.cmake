include(FindPackageHandleStandardArgs)

if (NOT BOCHS_FOUND)
    find_program(BOCHS_PATH bochsdbg bochs)
    add_executable(Bochs::bochs IMPORTED)
    set_target_properties(Bochs::bochs PROPERTIES IMPORTED_LOCATION "${BOCHS_PATH}")

    find_package_handle_standard_args(bochs DEFAULT_MSG BOCHS_PATH)
endif()
