find_package(octomap REQUIRED)
set(Matlab_ROOT_DIR /usr/local/MATLAB/R2021a/)
find_package(Matlab REQUIRED ENG_LIBRARY MAT_LIBRARY)

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
target_link_libraries(octonav PUBLIC octomap)
target_compile_features(octonav PUBLIC cxx_std_17)

# Matlab
add_executable(matlab_example matlab_example.cpp)
message(${Matlab_LIBRARIES})
target_link_libraries(matlab_example PRIVATE ${Matlab_LIBRARIES})
target_include_directories(matlab_example PRIVATE ${Matlab_INCLUDE_DIRS})