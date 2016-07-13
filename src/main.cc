#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>

#include "clogo.h"
#include "bopt.h"

using std::cout;
using std::endl;

double rosenbrock_2(
  double *i
) 
{
  double x = i[0], y = i[1];
  double min = -5.0;
  double max = 10.0;
  x = min + x * (max - min);
  y = min + y * (max - min);
  return -(100.0 * pow(y - x * x, 2.0) + pow(x * x - 1.0, 2.0));
}

double sin_helper(
  double x
)
{
  return (sin(13.0 * x)*sin(27.0 * x) + 1.0) / 2;
}

double sin_2(
  double *i
)
{
  double x = i[0], y = i[1];
  return sin_helper(x) * sin_helper(y);
}

int main() {
  std::ofstream of;
  of.open("soo.csv");
  eval_soo(of);
  of.close();
  of.open("logo.csv");
  eval_logo(of);
  of.close();
  of.open("bo.csv");
  eval_bo(of);
  of.close();
}
