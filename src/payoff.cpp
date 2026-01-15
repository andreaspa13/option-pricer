#include "pricer/payoff.hpp"

#include <algorithm>
#include <stdexcept>

namespace pricer {

double payoff(const Option& option, double spot_at_expiry) {
  if (spot_at_expiry < 0.0) {
    throw std::invalid_argument("Spot price must be non-negative");
  }

  const double K = option.strike;
  const double S = spot_at_expiry;

  switch (option.type) {
    case OptionType::Call:
      return std::max(S - K, 0.0);

    case OptionType::Put:
      return std::max(K - S, 0.0);

    default:
      throw std::invalid_argument("Unknown option type");
  }
}

} 
