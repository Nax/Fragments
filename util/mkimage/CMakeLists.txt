file(GLOB_RECURSE SOURCES "*.c" "*.cpp")
add_executable(mkimage ${SOURCES})
install(TARGETS mkimage RUNTIME DESTINATION bin)
