cmake_minimum_required(VERSION 3.5)
project(org_chart_test CXX)

add_executable(${PROJECT_NAME} test_main.cc test_controllers.cc)

# Add coverage flags for GCC (required for unit test generator)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -fprofile-arcs -ftest-coverage)
    target_link_libraries(${PROJECT_NAME} PRIVATE gcov)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE drogon)

ParseAndAddDrogonTests(${PROJECT_NAME})