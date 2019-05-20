add_subdirectory(boot)
add_subdirectory(libc)

install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/" DESTINATION include)
