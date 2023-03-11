cmake_minimum_required(VERSION 3.5)

project(BigDecimal LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_custom_target(BigDecimal
    SOURCES
    decimal.hpp
)

add_executable(BigDecimalTests
    tests/tests.cpp
)

add_executable(BigDecimalExamples
    examples/examples.cpp
)
