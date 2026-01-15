#pragma once

#include "option.hpp"

namespace pricer {

    // Returns payoff at expiry given terminal price S_T
    double payoff(const Option& option, double spot_at_expiry);
}