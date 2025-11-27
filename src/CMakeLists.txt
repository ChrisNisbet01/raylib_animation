# Define the executable with its source files in this directory.
# Note that the paths are relative to this CMakeLists.txt file.
add_executable(raylib_hello_world main.c animation1.c)

# Link the executable against the raylib and coroutine libraries.
# These targets are defined in the parent CMake scope.
target_link_libraries(raylib_hello_world PRIVATE raylib coroutine)
