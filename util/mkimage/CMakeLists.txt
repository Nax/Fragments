file(GLOB_RECURSE SOURCES "*.c" "*.cpp")
add_executable(mkimage ${SOURCES})
target_include_directories(mkimage PRIVATE "${CMAKE_SOURCE_DIR}/include/fs")
install(TARGETS mkimage RUNTIME DESTINATION bin)
