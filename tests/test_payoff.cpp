#include "pricer/payoff.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Call option payoff") {
  pricer::Option call{
    pricer::OptionType::Call,
    pricer::ExerciseStyle::European,
    100.0,
    1.0
  };

  REQUIRE(pricer::payoff(call, 120.0) == 20.0);
  REQUIRE(pricer::payoff(call, 100.0) == 0.0);
  REQUIRE(pricer::payoff(call, 80.0)  == 0.0);
}

TEST_CASE("Put option payoff") {
  pricer::Option put{
    pricer::OptionType::Put,
    pricer::ExerciseStyle::European,
    100.0,
    1.0
  };

  REQUIRE(pricer::payoff(put, 80.0)  == 20.0);
  REQUIRE(pricer::payoff(put, 100.0) == 0.0);
  REQUIRE(pricer::payoff(put, 120.0) == 0.0);
}

TEST_CASE("Invalid inputs throw exceptions") {
  pricer::Option call{
    pricer::OptionType::Call,
    pricer::ExerciseStyle::European,
    100.0,
    1.0
  };

  REQUIRE_THROWS(pricer::payoff(call, -1.0));
}
