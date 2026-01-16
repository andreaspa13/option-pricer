#pragma once

namespace pricer {

struct MarketData {
  double spot{0.0};   // S0
  double rate{0.0};   // r (annual, continuously compounded in my formula)
  double vol{0.0};    // sigma (annual volatility)
};

} 
