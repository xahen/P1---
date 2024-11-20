# Add package_distribution as a library
add_library(package_distribution package_distribution.c)
target_include_directories(package_distribution PUBLIC .)

# Add executable file
add_executable(run_program run_program.c)
target_link_libraries(run_program package_distribution)