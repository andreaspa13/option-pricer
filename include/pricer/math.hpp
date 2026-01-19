#pragma once
#include <cmath>

namespace pricer {

// Standard normal CDF N(x)
inline double norm_cdf(double x) {
  return 0.5 * (1.0 + std::erf(x / std::sqrt(2.0)));
}

inline double norm_pdf(double x) {
  static constexpr double inv_sqrt_2pi = 0.39894228040143267794; // 1/sqrt(2*pi)
  return inv_sqrt_2pi * std::exp(-0.5 * x * x);
}


} // namespace pricer
