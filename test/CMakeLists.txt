include(FetchContent)
FetchContent_Declare(mtest
        URL https://github.com/MortenSchou/mtest/archive/refs/tags/v0.2.0.zip
        URL_HASH SHA256=bec1e90fb00a5bc155de12ed5ed39ea5d1a1b6fcfb6c80cce5ad3e38e360248c
)
FetchContent_MakeAvailable(mtest)

# We create a test driver executable
add_executable(test_package_distribution test_package_distribution.c)
# We link the library to the test driver
target_link_libraries(test_package_distribution package_distribution mtest)

# We add the test executable to CTest
discover_tests(test_package_distribution)
