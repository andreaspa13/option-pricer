#include "pricer/black_scholes_greeks.hpp"

#include "pricer/math.hpp"
#include <cmath>
#include <stdexcept>

namespace pricer {

Greeks black_scholes_greeks(const MarketData& mkt, const Option& opt) {
  if (opt.style != ExerciseStyle::European) {
    throw std::invalid_argument("Black–Scholes Greeks support European options only");
  }
  if (mkt.spot <= 0.0) {
    throw std::invalid_argument("Spot must be > 0");
  }
  if (opt.strike <= 0.0) {
    throw std::invalid_argument("Strike must be > 0");
  }
  if (opt.expiry <= 0.0) {
    throw std::invalid_argument("Expiry must be > 0 for Greeks");
  }
  if (mkt.vol <= 0.0) {
    throw std::invalid_argument("Volatility must be > 0 for Greeks");
  }

  const double S = mkt.spot;
  const double K = opt.strike;
  const double r = mkt.rate;
  const double T = opt.expiry;
  const double sigma = mkt.vol;

  const double sqrtT = std::sqrt(T);
  const double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrtT);
  const double d2 = d1 - sigma * sqrtT;

  const double Nd1 = norm_cdf(d1);
  const double Nd2 = norm_cdf(d2);
  const double pdf_d1 = norm_pdf(d1);
  const double disc = std::exp(-r * T);

  Greeks g{};

  // Common Greeks
  g.gamma = pdf_d1 / (S * sigma * sqrtT);
  g.vega  = S * pdf_d1 * sqrtT;

  switch (opt.type) {
    case OptionType::Call: {
      g.delta = Nd1;
      g.theta = -(S * pdf_d1 * sigma) / (2.0 * sqrtT) - r * K * disc * Nd2;
      g.rho   = K * T * disc * Nd2;
      break;
    }
    case OptionType::Put: {
      g.delta = Nd1 - 1.0;
      g.theta = -(S * pdf_d1 * sigma) / (2.0 * sqrtT) + r * K * disc * norm_cdf(-d2);
      g.rho   = -K * T * disc * norm_cdf(-d2);
      break;
    }
    default:
      throw std::invalid_argument("Unknown option type");
  }

  return g;
}

} // namespace pricer
