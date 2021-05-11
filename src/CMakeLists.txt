find_package(octomap REQUIRED)
find_package(Matlab REQUIRED ENG_LIBRARY MAT_LIBRARY)

# The octomap-navigation library
file(GLOB OCTOMAP_NAVIGATION_HEADERS CONFIGURE_DEPENDS
    ../include/octonav/*.hpp
)

# Visualize
add_library(visualize visualize.cpp ${OCTOMAP_NAVIGATION_HEADERS})
target_link_libraries(visualize PUBLIC ${Matlab_LIBRARIES})
target_include_directories(visualize PUBLIC ${Matlab_INCLUDE_DIRS})
target_link_libraries(visualize PUBLIC octograph)

# Octograph
add_library(octograph OctoGraph.cpp ${OCTOMAP_NAVIGATION_HEADERS})
target_include_directories(octograph PUBLIC  ../include/)
target_link_libraries(octograph PUBLIC octomap)
target_compile_features(octograph PUBLIC cxx_std_17)



# Example provided by octomap devs
add_executable(simple_example simple_example.cpp  ${OCTOMAP_NAVIGATION_HEADERS})
target_link_libraries(simple_example PRIVATE visualize)

