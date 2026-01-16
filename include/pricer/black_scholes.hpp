#pragma once

#include "market.hpp"
#include "option.hpp"

namespace pricer {

// Prices a European option using Black–Scholes (assumes no dividends).
double black_scholes_price(const MarketData& mkt, const Option& opt);

} // namespace pricer
