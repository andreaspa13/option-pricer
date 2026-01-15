#pragma once
#include "types.hpp"

namespace pricer {
    Struct Option {
        OptionType type;
        ExerciseStyle style{ExerciseStyle::European};
        double strike{0.0}; //K Strike price
        double expiry{0.0}; //T Time to expiry in years
    };
}