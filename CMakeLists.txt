cmake_minimum_required(VERSION 3.1...3.20)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(CMAKE_CXX_STANDARD 17)

project(octonav
    VERSION 1.0
    LANGUAGES CXX
)

add_subdirectory(src)
add_subdirectory(test)
