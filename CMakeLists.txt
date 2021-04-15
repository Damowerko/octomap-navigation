cmake_minimum_required(VERSION 3.1...3.20)
project(octomap-navigation
    VERSION 1.0
    LANGUAGES CXX
)

find_package(octomap REQUIRED)

add_executable(simple_example src/simple_example.cpp)
target_link_libraries(simple_example PRIVATE octomap)
target_include_directories(simple_example PRIVATE octomap)