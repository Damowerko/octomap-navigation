find_package(octomap REQUIRED)
find_package(Matlab REQUIRED ENG_LIBRARY MAT_LIBRARY)
find_package(Threads REQUIRED)

# The octomap-navigation library
file(GLOB OCTOMAP_NAVIGATION_HEADERS CONFIGURE_DEPENDS
    ../include/octonav/*.hpp
)

# Example provided by octomap devs
add_executable(simple_example simple_example.cpp  ${OCTOMAP_NAVIGATION_HEADERS})
target_link_libraries(simple_example PRIVATE octomap)
target_include_directories(simple_example PRIVATE octomap)
target_include_directories(simple_example PRIVATE ../include/)

add_library(octonav
    OctoGraph.cpp

    ${OCTOMAP_NAVIGATION_HEADERS}
)
target_include_directories(octonav PUBLIC 
    ../include/
)
target_link_libraries(octonav PUBLIC octomap)
# target_compile_features(octonav PUBLIC cxx_std_17)

# Visualize
add_library(visualize visualize.cpp ${OCTOMAP_NAVIGATION_HEADERS})
target_link_libraries(visualize PRIVATE ${Matlab_LIBRARIES})
target_include_directories(visualize PRIVATE 
    ../include/
)
target_include_directories(visualize PRIVATE ${Matlab_INCLUDE_DIRS})
target_link_libraries(visualize PRIVATE octomap)
target_include_directories(visualize PRIVATE octomap)
