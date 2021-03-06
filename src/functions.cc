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

vectord rescale_input(const vectord& v, double min, double max)
{
  assert(max > min);
  vectord result = v;
  for (size_t i = 0; i < result.size(); i++) {
    result(i) = min + result(i) * (max - min);
  }
  return result;
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
  .raw_fn = [](const vectord& i) {
    assert(i.size() == 1);
    return sin_helper(i[0]);
  },
  .fn_max = 0.975599,
  .max_loc = {0.867526},
  .min_edge = {0},
  .max_edge = {1}
};

Function f_sin_2 = {
  .name = "sin_2",
  .dim = 2,
  .raw_fn = [](const vectord& i) {
    assert(i.size() == 2);
    return sin_helper(i[0]) * sin_helper(i[1]);
  },
  .fn_max = 0.9517936893872353,
  .max_loc = {0.867526, 0.867526},
  .min_edge = {0, 0},
  .max_edge = {1, 1}
};

/***********************************************************
* peaks
***********************************************************/
Function f_peaks = {
  .name = "peaks",
  .dim = 2,
  .raw_fn = [](const vectord& i) {
    assert(i.size() == 2);
    double x = i(0);
    double y = i(1);
    double sum = 0.0;
    sum += 3.0 * pow(1.0 - x, 2.0) * exp(-(x * x) - pow(y + 1.0, 2.0))
           - 10.0 * (x / 5.0 - pow(x, 3.0) - pow(y, 5.0)) * exp(-(x * x) - (y * y))
           - 1.0 / 3.0 * exp(-pow(x + 1.0, 2.0) - (y * y));
    return sum;
  },
  .fn_max = 8.106189094771677,
  .max_loc = {-0.009318,1.581366},
  .min_edge = {-3, -3},
  .max_edge = {3, 3}
};

/***********************************************************
* branin
***********************************************************/
Function f_branin = {
  .name = "branin",
  .dim = 2,
  .raw_fn = [](const vectord& i) {
    assert(i.size() == 2);
    double x = i(0);
    double y = i(1);
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
  .max_loc = {-M_PI, 12.275},
  .min_edge = {-5, 0},
  .max_edge = {10, 15}
};

/***********************************************************
* rosenbrock
***********************************************************/
double rosenbrock(const vectord& x) {
  double sum = 0.0;
  for (size_t i = 0; i < x.size()-1; i++) {
    sum += 100.0 * pow(x(i+1) - x(i) * x(i), 2.0) 
           + pow(1.0 - x(i), 2.0);
  }
  return -sum;
}

Function f_rosenbrock_2 = {
  .name = "rosenbrock_2",
  .dim = 2,
  .raw_fn = [](const vectord& i) {
    assert(i.size() == 2);
    return rosenbrock(i);
  },
  .fn_max = 0.0,
  .max_loc = {1,1},
  .min_edge = {-5, -5},
  .max_edge = {10, 10}
};

Function f_rosenbrock_4 = {
  .name = "rosenbrock_4",
  .dim = 4,
  .raw_fn = [](const vectord& i) {
    assert(i.size() == 4);
    return rosenbrock(i);
  },
  .fn_max = 0.0,
  .max_loc = {1,1,1,1},
  .min_edge = {-5, -5, -5, -5},
  .max_edge = {10, 10, 10, 10}
};

Function f_rosenbrock_6 = {
  .name = "rosenbrock_6",
  .dim = 6,
  .raw_fn = [](const vectord& i) {
    assert(i.size() == 6);
    return rosenbrock(i);
  },
  .fn_max = 0.0,
  .max_loc = {1,1,1,1,1,1},
  .min_edge = {-5, -5, -5, -5, -5, -5},
  .max_edge = {10, 10, 10, 10, 10, 10}
};

Function f_rosenbrock_8 = {
  .name = "rosenbrock_8",
  .dim = 8,
  .raw_fn = [](const vectord& i) {
    assert(i.size() == 8);
    return rosenbrock(i);
  },
  .fn_max = 0.0,
  .max_loc = {1,1,1,1,1,1,1,1},
  .min_edge = {-5, -5, -5, -5, -5, -5, -5, -5},
  .max_edge = {10, 10, 10, 10, 10, 10, 10, 10}
};

Function f_rosenbrock_10 = {
  .name = "rosenbrock_10",
  .dim = 10,
  .raw_fn = [](const vectord& i) {
    assert(i.size() == 10);
    return rosenbrock(i);
  },
  .fn_max = 0.0,
  .max_loc = {1,1,1,1,1,1,1,1,1,1},
  .min_edge = {-5, -5, -5, -5, -5, -5, -5, -5, -5, -5},
  .max_edge = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10}
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
  .raw_fn = [](const vectord& input) {
    return hartman(input, alpha, a3, p3);
  },
  .fn_max = 3.86278,
  .max_loc = {0.114589,0.555649,0.852547},
  .min_edge = {0, 0, 0},
  .max_edge = {1, 1, 1}
};

Function f_hartman_6 = {
  .name = "hartman_6",
  .dim = 6,
  .raw_fn = [](const vectord& input) {
    return hartman(input, alpha, a6, p6);
  },
  .fn_max = 3.32237,
  .max_loc = {0.20169, 0.150011, 0.476874, 0.275332, 0.311652, 0.6573},
  .min_edge = {0, 0, 0, 0, 0, 0},
  .max_edge = {1, 1, 1, 1, 1, 1}
};

/***********************************************************
* shekel
***********************************************************/
vectord b = set_vector({1, 2, 2, 4, 4, 6, 3, 7, 5, 5});
matrixd c = set_matrix(4, 10, { 4, 1, 8, 6, 3, 2, 5, 8, 6, 7,
                                4, 1, 8, 6, 7, 9, 3, 1, 2, 3,
                                4, 1, 8, 6, 3, 2, 5, 8, 6, 7,
                                4, 1, 8, 6, 7, 9, 3, 1, 2, 3 });
double shekel(const vectord& in, size_t m)
{
  assert(in.size() == 4);
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
  .raw_fn = [](const vectord& input) {
    return shekel(input, 5);
  },
  .fn_max = 10.1532,
  .max_loc = {4, 4, 4, 4},
  .min_edge = {0, 0, 0, 0},
  .max_edge = {10, 10, 10, 10}
};

Function f_shekel_7 = {
  .name = "shekel_7",
  .dim = 4,
  .raw_fn = [](const vectord& input) {
    return shekel(input, 7);
  },
  .fn_max = 10.4029,
  .max_loc = {4, 4, 4, 4},
  .min_edge = {0, 0, 0, 0},
  .max_edge = {10, 10, 10, 10}
};

Function f_shekel_10 = {
  .name = "shekel_10",
  .dim = 4,
  .raw_fn = [](const vectord& input) {
    return shekel(input, 10);
  },
  .fn_max = 10.5364,
  .max_loc = {4, 4, 4, 4},
  .min_edge = {0, 0, 0, 0},
  .max_edge = {10, 10, 10, 10}
};

/***********************************************************
* rastrigin
***********************************************************/

double rastrigin(const vectord& in, size_t d)
{
  assert(in.size() == d);
  double r = 10.0*d;
  for (size_t i = 0; i < d; i++) {
    r += in(i)*in(i) - 10.0*cos(2.0*M_PI*in(i));
  }
  return -r;
}

Function f_rastrigin_2 = {
  .name = "rastrigin_2",
  .dim = 2,
  .raw_fn = [](const vectord& input) {
    return rastrigin(input, 2);
  },
  .fn_max = 0,
  .max_loc = {0,0},
  .min_edge = {-5.12, -5.12},
  .max_edge = {5.12, 5.12}
};

Function f_rastrigin_4 = {
  .name = "rastrigin_4",
  .dim = 4,
  .raw_fn = [](const vectord& input) {
    return rastrigin(input, 4);
  },
  .fn_max = 0,
  .max_loc = {0,0,0,0},
  .min_edge = {-5.12, -5.12, -5.12, -5.12},
  .max_edge = {5.12, 5.12, 5.12, 5.12}
};

Function f_rastrigin_6 = {
  .name = "rastrigin_6",
  .dim = 6,
  .raw_fn = [](const vectord& input) {
    return rastrigin(input, 6);
  },
  .fn_max = 0,
  .max_loc = {0,0,0,0,0,0},
  .min_edge = {-5.12, -5.12, -5.12, -5.12, -5.12, -5.12},
  .max_edge = {5.12, 5.12, 5.12, 5.12, 5.12, 5.12}
};

Function f_rastrigin_10 = {
  .name = "rastrigin_10",
  .dim = 10,
  .raw_fn = [](const vectord& input) {
    return rastrigin(input, 10);
  },
  .fn_max = 0,
  .max_loc = {0,0,0,0,0,0,0,0,0,0},
  .min_edge = {-5.12, -5.12, -5.12, -5.12, -5.12, -5.12, -5.12, -5.12, -5.12, -5.12},
  .max_edge = {5.12, 5.12, 5.12, 5.12, 5.12, 5.12, 5.12, 5.12, 5.12, 5.12}
};

/***********************************************************
* schwefel
***********************************************************/

double schwefel(const vectord& in, size_t d)
{
  assert(in.size() == d);
  double r = 418.9829*d;
  for (size_t i = 0; i < d; i++) {
    r -= in(i) * sin(sqrt(fabs(in(i))));
  }
  return -r;
}

Function f_schwefel_2 = {
  .name = "schwefel_2",
  .dim = 2,
  .raw_fn = [](const vectord& input) {
    return schwefel(input, 2);
  },
  .fn_max = 0,
  .max_loc = {420.9687,420.9687},
  .min_edge = {-500, -500},
  .max_edge = {500, 500}
};

Function f_schwefel_4 = {
  .name = "schwefel_4",
  .dim = 4,
  .raw_fn = [](const vectord& input) {
    return schwefel(input, 4);
  },
  .fn_max = 0,
  .max_loc = {420.9687,420.9687,420.9687,420.9687},
  .min_edge = {-500, -500, -500, -500},
  .max_edge = {500, 500, 500, 500}
};

Function f_schwefel_6 = {
  .name = "schwefel_6",
  .dim = 6,
  .raw_fn = [](const vectord& input) {
    return schwefel(input, 6);
  },
  .fn_max = 0,
  .max_loc = {420.9687,420.9687,420.9687,420.9687,420.9687,420.9687},
  .min_edge = {-500, -500, -500, -500, -500, -500},
  .max_edge = {500, 500, 500, 500, 500, 500}
};

Function f_schwefel_10 = {
  .name = "schwefel_10",
  .dim = 10,
  .raw_fn = [](const vectord& input) {
    return schwefel(input, 10);
  },
  .fn_max = 0,
  .max_loc = {420.9687,420.9687,420.9687,420.9687,420.9687,420.9687,420.9687,420.9687,420.9687,420.9687},
  .min_edge = {-500, -500, -500, -500, -500, -500, -500, -500, -500, -500},
  .max_edge = {500, 500, 500, 500, 500, 500, 500, 500, 500, 500}
};

/***********************************************************
* ackley
***********************************************************/
double ackley(const vectord& in, size_t d)
{
  assert(in.size() == d);
  double a = 20.0;
  double b = 0.2;
  double c = 2.0 * M_PI;
  double inv_d = 1.0/d;

  double sum1 = 0.0;
  for (size_t i = 0; i < d; i++) {
    sum1 += in(i)*in(i);
  }

  double sum2 = 0.0;
  for (size_t i = 0; i < d; i++) {
    sum2 += cos(c * in(i));
  }

  double r = 0.0;
  r += -a * exp(-b * sqrt(inv_d*sum1));
  r += -exp(inv_d*sum2);
  r += a + exp(1.0);
  return -r;
}

Function f_ackley_2 = {
  .name = "ackley_2",
  .dim = 2,
  .raw_fn = [](const vectord& input) {
    return ackley(input, 2);
  },
  .fn_max = 0,
  .max_loc = {0,0},
  .min_edge = {-32.768, -32.768},
  .max_edge = {32.768, 32.768}
};

Function f_ackley_4 = {
  .name = "ackley_4",
  .dim = 4,
  .raw_fn = [](const vectord& input) {
    return ackley(input, 4);
  },
  .fn_max = 0,
  .max_loc = {0,0,0,0},
  .min_edge = {-32.768, -32.768, -32.768, -32.768, -32.768, -32.768},
  .max_edge = {32.768, 32.768, 32.768, 32.768}
};

Function f_ackley_6 = {
  .name = "ackley_6",
  .dim = 6,
  .raw_fn = [](const vectord& input) {
    return ackley(input, 6);
  },
  .fn_max = 0,
  .max_loc = {0,0,0,0,0,0},
  .min_edge = {-32.768, -32.768, -32.768, -32.768, -32.768, -32.768},
  .max_edge = {32.768, 32.768, 32.768, 32.768, 32.768, 32.768}
};

Function f_ackley_10 = {
  .name = "ackley_10",
  .dim = 10,
  .raw_fn = [](const vectord& input) {
    return ackley(input, 10);
  },
  .fn_max = 0,
  .max_loc = {0,0,0,0,0,0,0,0,0,0},
  .min_edge = {-32.768, -32.768, -32.768, -32.768, -32.768, -32.768, -32.768, -32.768, -32.768, -32.768},
  .max_edge = {32.768, 32.768, 32.768, 32.768, 32.768, 32.768, 32.768, 32.768, 32.768, 32.768}
};
