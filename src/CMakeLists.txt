find_package(octomap REQUIRED)
find_package(Matlab REQUIRED ENG_LIBRARY MAT_LIBRARY)
if(UNIX)
    find_package(Threads REQUIRED)
endif()

# The octomap-navigation library
file(GLOB OCTONAV_HEADERS CONFIGURE_DEPENDS
    ../include/octonav/*.hpp
)

# Octonav
add_library(octonav 
    OctoGraph.cpp
    a_star_generic.cpp
    visualize.cpp
    ${OCTONAV_HEADERS}
)
target_include_directories(octonav 
    PUBLIC  
        ../include/
        ${OCTOMAP_INCLUDE_DIRS}
    PRIVATE
        ${Matlab_INCLUDE_DIRS}
)
target_link_libraries(octonav PUBLIC octomap PRIVATE ${Matlab_LIBRARIES})
if(UNIX)
    target_link_libraries(octonav PRIVATE Threads::Threads)
endif()
target_compile_features(octonav PUBLIC cxx_std_17)


# Example provided by octomap devs
add_executable(simple_example simple_example.cpp)
target_link_libraries(simple_example PRIVATE octomap)

# Main
add_executable(main main.cpp)
target_link_libraries(main PRIVATE octonav)


