#include "pricer/black_scholes.hpp"
#include "pricer/payoff.hpp"


#include "pricer/math.hpp"
#include <cmath>
#include <stdexcept>

namespace pricer {

double black_scholes_price(const MarketData& mkt, const Option& opt) {
  if (opt.style != ExerciseStyle::European) {
    throw std::invalid_argument("Black–Scholes supports European options only");
  }
  if (mkt.spot <= 0.0) {
    throw std::invalid_argument("Spot must be > 0");
  }
  if (opt.strike <= 0.0) {
    throw std::invalid_argument("Strike must be > 0");
  }
  if (opt.expiry < 0.0) {
    throw std::invalid_argument("Expiry must be >= 0");
  }
  if (mkt.vol < 0.0) {
    throw std::invalid_argument("Volatility must be >= 0");
  }

  const double S0 = mkt.spot;
  const double K  = opt.strike;
  const double r  = mkt.rate;
  const double T  = opt.expiry;
  const double sigma = mkt.vol;

  // If expiry is now, price is just payoff today (no discounting needed).
  if (T == 0.0) {
    return payoff(opt, S0);
  }

  // If vol is zero, the future price is deterministic under risk-neutral drift:
  // S_T = S0 * exp(rT). Option value = discounted payoff of that deterministic S_T.
  if (sigma == 0.0) {
    const double ST = S0 * std::exp(r * T);
    const double discounted = std::exp(-r * T) * payoff(opt, ST);
    return discounted;
  }

  const double sqrtT = std::sqrt(T);
  const double d1 = (std::log(S0 / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrtT);
  const double d2 = d1 - sigma * sqrtT;

  const double Nd1 = norm_cdf(d1);
  const double Nd2 = norm_cdf(d2);
  const double discK = K * std::exp(-r * T);

  switch (opt.type) {
    case OptionType::Call:
      return S0 * Nd1 - discK * Nd2;
    case OptionType::Put:
      // Put price can be derived similarly or from parity.
      return discK * norm_cdf(-d2) - S0 * norm_cdf(-d1);
    default:
      throw std::invalid_argument("Unknown option type");
  }
}

} // namespace pricer
