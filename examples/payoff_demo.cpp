#include "pricer/payoff.hpp"
#include <iostream>

int main() {
  pricer::Option call{pricer::OptionType::Call, pricer::ExerciseStyle::European, 100.0, 1.0};
  pricer::Option put {pricer::OptionType::Put,  pricer::ExerciseStyle::European, 100.0, 1.0};

  const double ST = 120.0;

  std::cout << "Call payoff at ST=" << ST << " is " << pricer::payoff(call, ST) << "\n";
  std::cout << "Put payoff at ST="  << ST << " is " << pricer::payoff(put,  ST) << "\n";
}
