#pragma once

namespace pricer {

struct Greeks {
  double delta{0.0}; // dV/dS
  double gamma{0.0}; // d2V/dS2
  double vega{0.0};  // dV/dsigma
  double theta{0.0}; // dV/dT
  double rho{0.0};   // dV/dr
};

} 
