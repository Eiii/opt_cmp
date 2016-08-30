#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>

#include "clogo.h"
#include "bopt.h"

using std::cout;
using std::endl;

double s_angle;

void rotate(double deg, double *x, double *y)
{
  double rad = deg / 360.0;
  double new_x = cos(rad) * *x  - sin(rad) * *y;
  double new_y = sin(rad) * *x  + cos(rad) * *y;
  *x = new_x;
  *y = new_y;
}

double rosenbrock_2(
  double *i
) 
{
  double x = i[0], y = i[1];
  double min = -5.0;
  double max = 10.0;
  x = min + x * (max - min);
  y = min + y * (max - min);
  rotate(s_angle, &x, &y);
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

void comp() 
{
  s_angle = 0.0;
  {
    cout << "SOO" << endl;
    std::ofstream of("soo.csv");
    eval_soo(of);
    of.close();
  }
  {
    cout << "LOGO" << endl;
    std::ofstream of("logo.csv");
    eval_logo(of);
    of.close();
  }
  {
    cout << "BO" << endl;
    std::ofstream of("bo.csv");
    eval_good_bo(of);
    of.close();
  }
}

void output_angle_regrets(std::vector<double> v, std::string name)
{
  std::ofstream of;
  of.open(name);
  for (size_t i = 0; i < v.size(); i++) {
    of << i << "\t" << v[i] << endl;  
  }
  of.close();
}

void rot()
{
  //LOGO
  std::vector<double> logo_regrets, bo_regrets;
  constexpr int step = 10;
  for (int i = 0; i < 360; i += step) {
    s_angle = (double)i;
    double logo = eval_logo_final();
    logo_regrets.push_back(logo);

    double bo = eval_good_bo_final();
    bo_regrets.push_back(bo);
  }

  output_angle_regrets(logo_regrets, "rot_logo.csv");
  output_angle_regrets(bo_regrets, "rot_bo.csv");

}

int main() {
  comp();
}
