cmake_minimum_required(VERSION 3.11)
project(musconv VERSION 0.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic -g")

include_directories("include")
include_directories(${PROJECT_BINARY_DIR})
configure_file("cmake/version.h.in" "version.h")

add_executable(musconv 
  "src/flacenc.cpp"
  "src/main.cpp"
  "src/mpt.cpp"
  "src/musconv.cpp"
  "src/ope.cpp"
  "src/option.cpp"
  "src/reader.cpp"
  "src/selector.cpp"
  "src/writer.cpp"
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(musconv REQUIRED flac libopenmpt libopusenc)

include_directories("/usr/include/opus")

target_link_libraries(musconv ${musconv_LIBRARIES})
