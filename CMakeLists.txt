cmake_minimum_required(VERSION 3.14)
project(RaylibHelloWorld C)

find_package(OpenGL REQUIRED)

set(CMAKE_C_STANDARD 99)

# Use FetchContent to download and build raylib from source
include(FetchContent)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.0  # Use a specific version for reproducibility
)

# This makes the dependency available, downloading and adding it to the build
FetchContent_MakeAvailable(raylib)

# Add the executable
add_executable(raylib_hello_world src/main.c src/animation1.c)

# Link Raylib to the project. This also handles include directories.
target_link_libraries(raylib_hello_world PRIVATE raylib)
