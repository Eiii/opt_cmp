#include "functions.h"

CPUTimer objective_timer;

ObjectiveFn add_timer(const ObjectiveFn& fn)
{
  CPUTimer* timer_ptr = &objective_timer;
  auto new_fn = [timer_ptr, fn](const vectord& in) {
    timer_ptr->Start();
    auto result = fn(in);
    timer_ptr->Stop();
    return result;
  };
  return new_fn;
}

matrixd set_matrix(size_t rows, size_t cols, const std::vector<double>& vec)
{
  matrixd mat(rows, cols);
  assert(rows * cols == vec.size());
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      mat(i, j) = vec.at(i * cols + j);
    }
  }
  return mat;
}

vectord set_vector(const std::vector<double>& vec)
{
  vectord vd(vec.size());
  for (size_t i = 0; i < vec.size(); i++) {
    vd(i) = vec[i];
  }
  return vd;
}

/***********************************************************
* sin
***********************************************************/
double sin_helper(double x)
{
  return (sin(13.0*x)*sin(27.0*x)+1.0)/2.0;
}

Function f_sin_1 = {
  .name = "sin_1",
  .dim = 1,
  .fn = [](const vectord& i) {
    assert(i.size() == 1);
    return sin_helper(i[0]);
  },
  .fn_max = 0.975599,
  .max_loc = {0.867526}
};

Function f_sin_2 = {
  .name = "sin_2",
  .dim = 2,
  .fn = [](const vectord& i) {
    assert(i.size() == 2);
    return sin_helper(i[0]) * sin_helper(i[1]);
  },
  .fn_max = 0.9517936893872353,
  .max_loc = {0.867526, 0.867526}
};

/***********************************************************
* peaks
***********************************************************/
Function f_peaks = {
  .name = "peaks",
  .dim = 2,
  .fn = [](const vectord& i) {
    assert(i.size() == 2);
    double x = -3 + i(0) * (3 + 3);
    double y = -3 + i(1) * (3 + 3);
    double sum = 0.0;
    sum += 3.0 * pow(1.0 - x, 2.0) * exp(-(x * x) - pow(y + 1.0, 2.0))
           - 10.0 * (x / 5.0 - pow(x, 3.0) - pow(y, 5.0)) * exp(-(x * x) - (y * y))
           - 1.0 / 3.0 * exp(-pow(x + 1.0, 2.0) - (y * y));
    return sum;
  },
  .fn_max = 8.106189094771677,
  .max_loc = {0.498447,0.763561}
};

/***********************************************************
* branin
***********************************************************/
Function f_branin = {
  .name = "branin",
  .dim = 2,
  .fn = [](const vectord& i) {
    assert(i.size() == 2);
    double x = -5.0 + i(0) * (10.0 + 5.0);
    double y = i(1) * 15.0;
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
  },
  .fn_max = -0.397887,
  .max_loc = {0.123894,0.818333}
};

/***********************************************************
* rosenbrock
***********************************************************/
double rosenbrock(const vectord& input) {
  double min = -5.0; double max = 10.0;
  vectord it(input);
  for (auto& x : it) x = min + x * (max - min);
  double sum = 0.0;
  for (size_t i = 0; i < it.size()-1; i++) {
    sum += 100.0 * pow(it(i+1) - it(i) * it(i), 2.0) 
           + pow(1.0 - it(i), 2.0);
  }
  return -sum;
}

Function f_rosenbrock_2 = {
  .name = "rosenbrock_2",
  .dim = 2,
  .fn = [](const vectord& i) {
    assert(i.size() == 2);
    return rosenbrock(i);
  },
  .fn_max = 0.0,
  .max_loc = {0.4,0.4}
};

Function f_rosenbrock_4 = {
  .name = "rosenbrock_4",
  .dim = 4,
  .fn = [](const vectord& i) {
    assert(i.size() == 4);
    return rosenbrock(i);
  },
  .fn_max = 0.0,
  .max_loc = {0.4,0.4,0.4,0.4}
};

Function f_rosenbrock_6 = {
  .name = "rosenbrock_6",
  .dim = 6,
  .fn = [](const vectord& i) {
    assert(i.size() == 6);
    return rosenbrock(i);
  },
  .fn_max = 0.0,
  .max_loc = {0.4,0.4,0.4,0.4,0.4,0.4}
};

Function f_rosenbrock_8 = {
  .name = "rosenbrock_8",
  .dim = 8,
  .fn = [](const vectord& i) {
    assert(i.size() == 8);
    return rosenbrock(i);
  },
  .fn_max = 0.0,
  .max_loc = {0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4}
};

Function f_rosenbrock_10 = {
  .name = "rosenbrock_10",
  .dim = 10,
  .fn = [](const vectord& i) {
    assert(i.size() == 10);
    return rosenbrock(i);
  },
  .fn_max = 0.0,
  .max_loc = {0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4,0.4}
};

/***********************************************************
* hartman
***********************************************************/
vectord alpha = set_vector({1.0, 1.2, 3.0, 3.2});
matrixd a3 = set_matrix(4, 3, { 3.0, 10, 30, 
                                0.1, 10, 35, 
                                3.0, 10, 30, 
                                0.1, 10, 35 });
matrixd p3 = set_matrix(4, 3, { 3689, 1170, 2673, 
                                4699, 4387, 7470, 
                                1091, 8732, 5547, 
                                 381, 5743, 8828 });
matrixd a6 = set_matrix(4, 6, { 10,     3,   17, 3.5, 1.7,  8,
                                0.05,  10,   17, 0.1,   8, 14,
                                3,    3.5,  1.7,  10,  17,  8,
                                17,     8, 0.05,  10, 0.1, 14 });
matrixd p6 = set_matrix(4, 6, { 1312, 1696, 5569,  124, 8283, 5886,
                                2329, 4135, 8307, 3736, 1004, 9991,
                                2348, 1451, 3522, 2883, 3047, 6650,
                                4047, 8828, 8732, 5743, 1091,  381 });

double hartman(const vectord& input, 
               const vectord& alpha, const matrixd& a, const matrixd& p)
{
  assert(input.size() == a.size2() && input.size() == p.size2());
  double sum = 0.0;
  for (size_t i = 0; i < 4; i++) {
    double inner = 0.0;
    for (size_t j = 0; j < input.size(); j++) {
      inner += a(i,j) * pow(input[j] - 1e-4 * p(i,j), 2.0);
    }
    sum += alpha(i) * exp(-inner);
  }
  return sum;
}
                          
Function f_hartman_3 = {
  .name = "hartman_3",
  .dim = 3,
  .fn = [](const vectord& input) {
    return hartman(input, alpha, a3, p3);
  },
  .fn_max = 3.86278,
  .max_loc = {0.114589,0.555649,0.852547}
};

Function f_hartman_6 = {
  .name = "hartman_6",
  .dim = 6,
  .fn = [](const vectord& input) {
    return hartman(input, alpha, a6, p6);
  },
  .fn_max = 3.32237,
  .max_loc = {0.20169, 0.150011, 0.476874, 0.275332, 0.311652, 0.6573}
};

/***********************************************************
* shekel
***********************************************************/
vectord b = set_vector({1, 2, 2, 4, 4, 6, 3, 7, 5, 5});
matrixd c = set_matrix(4, 10, { 4, 1, 8, 6, 3 ,2, 5, 8, 6, 7,
                                4, 1, 8, 6, 7, 9, 3, 1, 2, 3,
                                4, 1, 8, 6, 3, 2, 5, 8, 6, 7,
                                4, 1, 8, 6, 7, 9, 3, 1, 2, 3 });
double shekel(const vectord& input, size_t m)
{
  assert(input.size() == 4);
  vectord in(input);
  for (auto& x : in) x = x * 10.0;
  double sum = 0.0;
  for (size_t i = 0; i < m; i++) {
    double inner = b(i) / 10.0;
    for (size_t j = 0; j < 4; j++) {
      inner += pow(in(j) - c(j,i), 2.0); 
    }
    sum += 1.0 / inner;
  }
  return sum;
}

Function f_shekel_5 = {
  .name = "shekel_5",
  .dim = 4,
  .fn = [](const vectord& input) {
    return shekel(input, 5);
  },
  .fn_max = 10.1532,
  .max_loc = {0.4, 0.4, 0.4, 0.4}
};

Function f_shekel_7 = {
  .name = "shekel_7",
  .dim = 4,
  .fn = [](const vectord& input) {
    return shekel(input, 7);
  },
  .fn_max = 10.4029,
  .max_loc = {0.4, 0.4, 0.4, 0.4}
};

Function f_shekel_10 = {
  .name = "shekel_10",
  .dim = 4,
  .fn = [](const vectord& input) {
    return shekel(input, 10);
  },
  .fn_max = 10.5364,
  .max_loc = {0.4, 0.4, 0.4, 0.4}
};
