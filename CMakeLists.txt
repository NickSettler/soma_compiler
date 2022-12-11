cmake_minimum_required(VERSION 3.23)
project(soma_compiler)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

enable_testing()


add_executable(
        tests
        tests/main.cpp)


target_link_libraries(
        tests
        PRIVATE
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)

add_executable(
        soma
        src/main.c
        src/lexical_analysis.c
        src/lexical_analysis.h)
