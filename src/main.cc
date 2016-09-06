#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>
#include <vector>

#include "bopt.h"
#include "functions.h"

constexpr int samples = 500;

std::vector<const Function*> functions = { 
  &f_sin_1, &f_sin_2,
  &f_peaks,
  &f_branin,
  &f_rosenbrock_2,
  &f_hartman_3,
  &f_shekel_5, &f_shekel_7, &f_shekel_10,
  &f_hartman_6,
  &f_rosenbrock_10
};

void comp() 
{
  std::ofstream of("bo.csv");
  for (const auto& fn : functions) {
    std::cout << fn->name << std::endl;
    eval_good_bo(*fn, samples, of);
  }
  of.close();
}

int main() {
  comp();
}
