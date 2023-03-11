cmake_minimum_required(VERSION 3.5)

project(StrictDecimal LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_custom_target(StrictDecimal
    SOURCES
    decimal.hpp
)

add_executable(StrictDecimalTests
    tests/decimal_tests.hpp
    tests/decimal_tests.cpp
    tests/main.cpp
    tests/ranged_decimal_tests.hpp
    tests/ranged_decimal_tests.cpp
    tests/test_macros.hpp
)

add_executable(StrictDecimalExamples
    examples/examples.cpp
)
