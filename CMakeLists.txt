cmake_minimum_required(VERSION 3.11)
project(musconv VERSION 0.5)

# libvgm
option(BUILD_TESTS "build test programs" OFF)
option(BUILD_PLAYER "build player application" OFF)
option(BUILD_VGM2WAV "build sample vgm2wav application" OFF)
set(LIBRARY_TYPE "SHARED" CACHE STRING "library build type (static/shared)")
add_subdirectory("third_party/libvgm")
link_directories("${CMAKE_BINARY_DIR}/third_party/libvgm/bin")

find_package(PkgConfig REQUIRED)
pkg_check_modules(musconv REQUIRED flac libopenmpt libopusenc libgme)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic -g")

include_directories(${PROJECT_BINARY_DIR})
configure_file("cmake/version.h.in" "version.h")

include_directories("include")
add_executable(musconv 
  "src/fileutil.cpp"
  "src/flacenc.cpp"
  "src/gme.cpp"
  "src/main.cpp"
  "src/mpt.cpp"
  "src/musconv.cpp"
  "src/ope.cpp"
  "src/option.cpp"
  "src/reader.cpp"
  "src/selector.cpp"
  "src/vgm.cpp"
  "src/writer.cpp"
)

include_directories("/usr/include/opus")
target_link_libraries(musconv ${musconv_LIBRARIES} vgm-player vgm-utils)
