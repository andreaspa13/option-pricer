#include "pricer/payoff.hpp"
#include <gtest/gtest.h>

TEST(PayoffTests, CallPayoff) {
  pricer::Option call{
    pricer::OptionType::Call,
    pricer::ExerciseStyle::European,
    100.0,
    1.0
  };

  EXPECT_EQ(pricer::payoff(call, 120.0), 20.0);
  EXPECT_EQ(pricer::payoff(call, 100.0), 0.0);
  EXPECT_EQ(pricer::payoff(call, 80.0),  0.0);
}

TEST(PayoffTests, PutPayoff) {
  pricer::Option put{
    pricer::OptionType::Put,
    pricer::ExerciseStyle::European,
    100.0,
    1.0
  };

  EXPECT_EQ(pricer::payoff(put, 80.0),  20.0);
  EXPECT_EQ(pricer::payoff(put, 100.0), 0.0);
  EXPECT_EQ(pricer::payoff(put, 120.0), 0.0);
}

TEST(PayoffTests, InvalidInputsThrow) {
  pricer::Option call{
    pricer::OptionType::Call,
    pricer::ExerciseStyle::European,
    100.0,
    1.0
  };

  EXPECT_THROW(pricer::payoff(call, -1.0), std::invalid_argument);
}
