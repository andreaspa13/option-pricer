cmake_minimum_required(VERSION 3.20)

project(option_pricer
  VERSION 0.1.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# GoogleTest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)

# Prevent GoogleTest from overriding our compiler/linker settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_library(pricer
  src/payoff.cpp
  src/black_scholes.cpp
)
target_include_directories(pricer PUBLIC include)

add_executable(tests
  tests/test_payoff.cpp
  tests/test_black_scholes.cpp
)
target_link_libraries(tests PRIVATE pricer GTest::gtest_main)

include(CTest)

include(GoogleTest)
gtest_discover_tests(tests)

add_executable(payoff_demo
  examples/payoff_demo.cpp
)
target_link_libraries(payoff_demo PRIVATE pricer)
