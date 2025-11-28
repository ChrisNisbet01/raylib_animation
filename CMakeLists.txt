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

# Define an imported target for the pre-built coroutine library
add_library(coroutine STATIC IMPORTED)
set_target_properties(coroutine PROPERTIES
    IMPORTED_LOCATION "/home/chris/projects/coroutine/build/libcoroutine.a"
    INTERFACE_INCLUDE_DIRECTORIES "/home/chris/projects/coroutine/include"
)

# Define an imported target for the pre-built easing_functions library
add_library(easing_functions STATIC IMPORTED)
set_target_properties(easing_functions PROPERTIES
    IMPORTED_LOCATION "/home/chris/projects/easing_functions/build/libeasing_functions.a"
    INTERFACE_INCLUDE_DIRECTORIES "/home/chris/projects/easing_functions/include"
)

# Add the 'src' directory which now contains its own CMakeLists.txt
add_subdirectory(src)
