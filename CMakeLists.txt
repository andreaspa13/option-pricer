cmake_minimum_required(VERSION 3.20)

project(option_pricer
  VERSION 0.1.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.6.0
)
FetchContent_MakeAvailable(Catch2)

add_library(pricer
  src/payoff.cpp
)
target_include_directories(pricer PUBLIC include)

add_executable(tests
  tests/test_payoff.cpp
)
target_link_libraries(tests PRIVATE pricer Catch2::Catch2WithMain)

include(CTest)
add_test(NAME payoff_tests COMMAND tests)
