#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>

#include "clogo.h"
#include "bopt.h"

using std::cout;
using std::endl;

double rosenbrock_2(double *i) 
{
  double x = i[0], y = i[1];
  double min = -5.0;
  double max = 10.0;
  x = min + x * (max - min);
  y = min + y * (max - min);
  return -(100.0 * pow(y - x * x, 2.0) + pow(x - 1.0, 2.0));
}

double rosenbrock_10(double *in) 
{
  double i[10];
  double min = -5.0;
  double max = 10.0;
  for (int x = 0; x < 10; x++) {
    i[x] = min + in[x] * (max - min);
  }
  double sum = 0.0;
  for (size_t x = 0; x < 10-1; x++) {
    sum += 100.0 * pow(i[x+1] - i[x] * i[x], 2.0) 
           + pow(1.0 - i[x], 2.0);
  }
  return -sum;
}

double sin_helper(double x)
{
  return (sin(13.0 * x)*sin(27.0 * x) + 1.0) / 2;
}

double sin_2(double *i)
{
  double x = i[0], y = i[1];
  return sin_helper(x) * sin_helper(y);
}

double peaks(double *i)
{
    double x = -3 + i[0] * (3 + 3);
    double y = -3 + i[1] * (3 + 3);
    double sum = 0.0;
    sum += 3.0 * pow(1.0 - x, 2.0) * exp(-(x * x) - pow(y + 1.0, 2.0))
           - 10.0 * (x / 5.0 - pow(x, 3.0) - pow(y, 5.0)) * exp(-(x * x) - (y * y))
           - 1.0 / 3.0 * exp(-pow(x + 1.0, 2.0) - (y * y));
    return sum;
}

double branin(double *i)
{
    double x = -5.0 + i[0] * (10.0 + 5.0);
    double y = i[1] * 15.0;
    double a = 1.0;
    double b = 5.1 / (4.0 * M_PI * M_PI);
    double c = 5.0 / M_PI;
    double r = 6.0;
    double s = 10.0;
    double t = 1.0 / (8.0 * M_PI);
    double val = a * pow(y - b * x * x + c * x - r, 2.0) 
                 + s * (1.0 - t) * cos(x) 
                 + s;
    return -val;
}

void comp() 
{
  {
    cout << "BO" << endl;
    std::ofstream of("bo.csv");
    eval_good_bo(of);
    of.close();
  }
}

int main() {
  comp();
}
