find_package(octomap REQUIRED)

# Example provided by octomap devs
add_executable(simple_example simple_example.cpp)
target_link_libraries(simple_example PRIVATE octomap)
target_include_directories(simple_example PRIVATE octomap)

# The octomap-navigation library
file(GLOB OCTOMAP_NAVIGATION_HEADERS CONFIGURE_DEPENDS
    ../include/octonav/*.hpp
)
add_library(octonav
    OctoGraph.cpp
    ${OCTOMAP_NAVIGATION_HEADERS}
)
target_include_directories(octonav PUBLIC 
    ../include/
)
target_link_libraries(octonav PRIVATE octomap)
target_compile_features(octonav PUBLIC cxx_std_17)