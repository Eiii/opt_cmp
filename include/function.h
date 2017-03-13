#pragma once

#include "types.h"

struct Function {
  std::string name;
  int dim;
  ObjectiveFn raw_fn;
  double fn_max;
  std::vector<double> max_loc;
  std::vector<double> min_edge;
  std::vector<double> max_edge;
};

ObjectiveFn create_scaled_fn(const Function& f);
vectord scale_input(const vectord& v, const Function& f);
vectord unscale_input(const vectord& v, const Function& f);
Function tweak_edges(const Function& f, int seed);
