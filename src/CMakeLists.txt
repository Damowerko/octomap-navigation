find_package(octomap REQUIRED)
find_package(Matlab REQUIRED ENG_LIBRARY MAT_LIBRARY)

# Octonav
add_library(octonav 
    OctoGraph.cpp
    a_star_generic.cpp
    ../include/octonav/a_star.hpp
    ../include/octonav/GridGraph.hpp
    ../include/octonav/OctoGraph.hpp
)
target_include_directories(octonav PUBLIC  
    ../include/
    ${OCTOMAP_INCLUDE_DIRS}
)
target_link_libraries(octonav PUBLIC octomap)
target_compile_features(octonav PUBLIC cxx_std_17)


# visualize library
add_library(visualize visualize.cpp ../include/octonav/visualize.hpp)
target_include_directories(visualize PUBLIC ../include ${OCTOMAP_INCLUDE_DIRS} ${Matlab_INCLUDE_DIRS})
target_link_libraries(visualize PUBLIC ${Matlab_LIBRARIES})
if(UNIX)
    # Won't compile on linux without Threads.
    find_package(Threads REQUIRED)
    target_link_libraries(visualize PRIVATE Threads::Threads)
endif()

# Example provided by octomap devs
add_executable(simple_example simple_example.cpp)
target_link_libraries(simple_example PUBLIC octomap)

# Main
add_executable(main main.cpp)
target_link_libraries(main PRIVATE octonav visualize)


