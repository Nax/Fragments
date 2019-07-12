file(GLOB_RECURSE SOURCES "*.c" "*.cpp")
add_executable(mkrdisk ${SOURCES})
target_include_directories(mkrdisk PRIVATE "${CMAKE_SOURCE_DIR}/include/fs")
install(TARGETS mkrdisk RUNTIME DESTINATION bin)
