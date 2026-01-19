#include "pricer/black_scholes.hpp"
#include "pricer/black_scholes_greeks.hpp"
#include <gtest/gtest.h>
#include <cmath>

namespace {

double price_with_spot(const pricer::MarketData& m, const pricer::Option& o, double S) {
  pricer::MarketData bumped = m;
  bumped.spot = S;
  return pricer::black_scholes_price(bumped, o);
}

double price_with_vol(const pricer::MarketData& m, const pricer::Option& o, double vol) {
  pricer::MarketData bumped = m;
  bumped.vol = vol;
  return pricer::black_scholes_price(bumped, o);
}

double price_with_rate(const pricer::MarketData& m, const pricer::Option& o, double r) {
  pricer::MarketData bumped = m;
  bumped.rate = r;
  return pricer::black_scholes_price(bumped, o);
}

double price_with_expiry(const pricer::MarketData& m, const pricer::Option& o, double T) {
  pricer::Option bumped = o;
  bumped.expiry = T;
  return pricer::black_scholes_price(m, bumped);
}

} // namespace

TEST(GreeksTests, CallGreeksMatchFiniteDifferences) {
  const pricer::MarketData mkt{100.0, 0.05, 0.20};
  const pricer::Option call{pricer::OptionType::Call, pricer::ExerciseStyle::European, 100.0, 1.0};

  const pricer::Greeks g = pricer::black_scholes_greeks(mkt, call);

  const double S0 = mkt.spot;
  const double hS = 1e-3 * S0;
  const double hV = 1e-4;
  const double hR = 1e-4;
  const double hT = 1e-4;

  const double C_Sp = price_with_spot(mkt, call, S0 + hS);
  const double C_Sm = price_with_spot(mkt, call, S0 - hS);
  const double C_S0 = pricer::black_scholes_price(mkt, call);

  const double delta_fd = (C_Sp - C_Sm) / (2.0 * hS);
  const double gamma_fd = (C_Sp - 2.0 * C_S0 + C_Sm) / (hS * hS);

  const double C_Vp = price_with_vol(mkt, call, mkt.vol + hV);
  const double C_Vm = price_with_vol(mkt, call, mkt.vol - hV);
  const double vega_fd = (C_Vp - C_Vm) / (2.0 * hV);

  const double C_Rp = price_with_rate(mkt, call, mkt.rate + hR);
  const double C_Rm = price_with_rate(mkt, call, mkt.rate - hR);
  const double rho_fd = (C_Rp - C_Rm) / (2.0 * hR);

  // Our theta is dV/dT where T is time-to-expiry
  const double C_Tp = price_with_expiry(mkt, call, call.expiry + hT);
  const double C_Tm = price_with_expiry(mkt, call, call.expiry - hT);
  const double theta_fd = -(C_Tp - C_Tm) / (2.0 * hT);

  EXPECT_NEAR(g.delta, delta_fd, 1e-6);
  EXPECT_NEAR(g.gamma, gamma_fd, 1e-6);
  EXPECT_NEAR(g.vega,  vega_fd,  1e-4);
  EXPECT_NEAR(g.rho,   rho_fd,   1e-4);
  EXPECT_NEAR(g.theta, theta_fd, 1e-4);
}

TEST(GreeksTests, PutGreeksMatchFiniteDifferences) {
  const pricer::MarketData mkt{100.0, 0.05, 0.20};
  const pricer::Option put{pricer::OptionType::Put, pricer::ExerciseStyle::European, 100.0, 1.0};

  const pricer::Greeks g = pricer::black_scholes_greeks(mkt, put);

  const double S0 = mkt.spot;
  const double hS = 1e-3 * S0;
  const double hV = 1e-4;
  const double hR = 1e-4;
  const double hT = 1e-4;

  const double P_Sp = price_with_spot(mkt, put, S0 + hS);
  const double P_Sm = price_with_spot(mkt, put, S0 - hS);
  const double P_S0 = pricer::black_scholes_price(mkt, put);

  const double delta_fd = (P_Sp - P_Sm) / (2.0 * hS);
  const double gamma_fd = (P_Sp - 2.0 * P_S0 + P_Sm) / (hS * hS);

  const double P_Vp = price_with_vol(mkt, put, mkt.vol + hV);
  const double P_Vm = price_with_vol(mkt, put, mkt.vol - hV);
  const double vega_fd = (P_Vp - P_Vm) / (2.0 * hV);

  const double P_Rp = price_with_rate(mkt, put, mkt.rate + hR);
  const double P_Rm = price_with_rate(mkt, put, mkt.rate - hR);
  const double rho_fd = (P_Rp - P_Rm) / (2.0 * hR);

  const double P_Tp = price_with_expiry(mkt, put, put.expiry + hT);
  const double P_Tm = price_with_expiry(mkt, put, put.expiry - hT);
  const double theta_fd = -(P_Tp - P_Tm) / (2.0 * hT);

  EXPECT_NEAR(g.delta, delta_fd, 1e-6);
  EXPECT_NEAR(g.gamma, gamma_fd, 1e-6);
  EXPECT_NEAR(g.vega,  vega_fd,  1e-4);
  EXPECT_NEAR(g.rho,   rho_fd,   1e-4);
  EXPECT_NEAR(g.theta, theta_fd, 1e-4);
}
