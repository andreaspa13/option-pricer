#include "pricer/black_scholes.hpp"
#include "pricer/payoff.hpp"
#include <gtest/gtest.h>
#include <cmath>

TEST(BlackScholesTests, MatchesReferenceValues) {
  pricer::MarketData mkt{100.0, 0.05, 0.20};

  pricer::Option call{pricer::OptionType::Call, pricer::ExerciseStyle::European, 100.0, 1.0};
  pricer::Option put {pricer::OptionType::Put,  pricer::ExerciseStyle::European, 100.0, 1.0};

  const double call_price = pricer::black_scholes_price(mkt, call);
  const double put_price  = pricer::black_scholes_price(mkt, put);

  EXPECT_NEAR(call_price, 10.4506, 1e-4);
  EXPECT_NEAR(put_price,   5.5735, 1e-4);
}

TEST(BlackScholesTests, SatisfiesPutCallParity) {
  const double S0 = 100.0;
  const double K = 100.0;
  const double r = 0.05;
  const double T = 1.0;
  const double sigma = 0.20;

  pricer::MarketData mkt{S0, r, sigma};
  pricer::Option call{pricer::OptionType::Call, pricer::ExerciseStyle::European, K, T};
  pricer::Option put {pricer::OptionType::Put,  pricer::ExerciseStyle::European, K, T};

  const double C = pricer::black_scholes_price(mkt, call);
  const double P = pricer::black_scholes_price(mkt, put);

  const double rhs = S0 - K * std::exp(-r * T);

  EXPECT_NEAR(C - P, rhs, 1e-10);
}

TEST(BlackScholesTests, ExpiryNowEqualsPayoff) {
  pricer::MarketData mkt{90.0, 0.05, 0.20};

  pricer::Option call{pricer::OptionType::Call, pricer::ExerciseStyle::European, 100.0, 0.0};
  pricer::Option put {pricer::OptionType::Put,  pricer::ExerciseStyle::European, 100.0, 0.0};

  EXPECT_EQ(pricer::black_scholes_price(mkt, call), pricer::payoff(call, mkt.spot));
  EXPECT_EQ(pricer::black_scholes_price(mkt, put),  pricer::payoff(put,  mkt.spot));
}

TEST(BlackScholesTests, ZeroVolUsesDeterministicForward) {
  const double S0 = 100.0;
  const double K  = 110.0;
  const double r  = 0.05;
  const double T  = 1.0;

  pricer::MarketData mkt{S0, r, 0.0};
  pricer::Option call{pricer::OptionType::Call, pricer::ExerciseStyle::European, K, T};

  const double ST = S0 * std::exp(r * T);
  const double expected = std::exp(-r * T) * pricer::payoff(call, ST);

  EXPECT_NEAR(pricer::black_scholes_price(mkt, call), expected, 1e-12);
}
