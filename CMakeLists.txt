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
        tests/main.cpp
        tests/lexical_analysis_tests.cpp
        tests/syntax_analysis_tests.cpp
        tests/semantic_analysis_tests.cpp)


target_link_libraries(
        tests
        PRIVATE
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)

add_executable(
        soma
        src/main.cpp
        src/optimiser.cpp src/optimiser.h
        src/util/enum.h
        src/util/types.h
        src/util/errors.h
        src/lexical_analysis.cpp src/lexical_analysis.h
        src/syntax_analysis.cpp src/syntax_analysis.h
        src/symbol_table.cpp src/symbol_table.h
        src/semantic_analysis.cpp src/semantic_analysis.h )
