#include <iostream>
#include <cassert>
#include <cmath>
#include <fstream>

#include "bopt.h"
#include "functions.h"

using std::cout;
using std::endl;

void comp() 
{
  {
    cout << "BO" << endl;
    std::ofstream of("bo.csv");
    eval_good_bo(f_rosenbrock_10, of);
    of.close();
  }
}

int main() {
  comp();
}
