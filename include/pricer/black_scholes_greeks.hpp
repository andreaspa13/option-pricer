#pragma once

#include "greeks.hpp"
#include "market.hpp"
#include "option.hpp"

namespace pricer {

// Analytic Greeks under Black–Scholes (European, no dividends).
Greeks black_scholes_greeks(const MarketData& mkt, const Option& opt);

} // namespace pricer
