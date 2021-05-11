cmake_minimum_required(VERSION 3.1...3.20)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")
set(CMAKE_CXX_STANDARD 17)

project(octomap-navigation
    VERSION 1.0
    LANGUAGES CXX
)

find_package(octomap CONFIG REQUIRED)

add_executable(simple_example src/simple_example.cpp)
target_link_libraries(simple_example PRIVATE octomap)
target_include_directories(simple_example PRIVATE octomap)