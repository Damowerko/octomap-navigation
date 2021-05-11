find_package(Catch2)
include(CTest)
include(Catch)

file(GLOB TEST_SRC "*.cpp")
add_executable(tests ${TEST_SRC})
target_link_libraries(tests PRIVATE Catch2::Catch2 octograph)
catch_discover_tests(tests)