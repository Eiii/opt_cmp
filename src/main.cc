#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>
#include <vector>

#include "functions.h"
#include "boharness.h"
#include "logoharness.h"

#include "cpplogo/logging.h" //TODO: This is just so we can disable logging!

constexpr int SAMPLES = 100;
constexpr int MAIN_SEED = 1337;

std::vector<const Function*> functions = { 
  /*
  &f_sin_1, &f_sin_2,
  */
  &f_peaks,
  /*
  &f_branin,
  */
  &f_rosenbrock_2,
  &f_hartman_3,
  /*
  &f_shekel_5, 
  */
  &f_shekel_7, 
  /*
  &f_shekel_10,
  &f_hartman_6,
  */
  &f_rosenbrock_10
};

void comp() 
{
  constexpr int NUM_ITERATIONS = 250;
  std::ofstream of("bo.csv");
  for (const auto& fn : functions) {
    BOHarness harness(*fn, MAIN_SEED);
    std::cout << harness.name() << " / " << fn->name << std::endl;
    harness.Evaluate(SAMPLES, NUM_ITERATIONS);
    harness.OutputResult(&of);
  }
  for (const auto& fn : functions) {
    LOGOHarness harness(*fn, MAIN_SEED);
    std::cout << harness.name() << " / " << fn->name << std::endl;
    harness.Evaluate(SAMPLES, NUM_ITERATIONS);
    harness.OutputResult(&of);
  }
  of.close();
}

int main() {
  init_logging(logo::info); //Disable cpplogo logging. Dumb.
  comp();
}
