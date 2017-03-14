#include "function.h"
#include <iostream>

ObjectiveFn create_scaled_fn(const Function& f)
{
  auto l = [f](const vectord& v) -> double {
    auto scaled = scale_input(v, f);
    return f.raw_fn(scaled);
  };  
  return l;
} /* create_scaled_fn() */

vectord scale_input(const vectord& v, const Function& f)
{
  assert(v.size() == static_cast<size_t>(f.dim));
  vectord result(f.dim);
  for (int i = 0; i < f.dim; i++) {
    assert(f.min_edge[i] < f.max_edge[i]);
    assert(0 <= v[i] && v[i] <= 1);
    result[i] = f.min_edge[i] + v[i]*(f.max_edge[i] - f.min_edge[i]);
  }
  return result;
} /* scale_input() */

vectord unscale_input(const vectord& v, const Function& f)
{
  assert(v.size() == static_cast<size_t>(f.dim));
  vectord result(f.dim);
  for (int i = 0; i < f.dim; i++) {
    result[i] = (v[i] - f.min_edge[i])/(f.max_edge[i] - f.min_edge[i]);
  }
  return result;
} /* unscale_input() */

Function tweak_edges(const Function& f, int seed)
{
  constexpr double buffer = 0.9;
  RandomEngine rng(seed);
  std::vector<double> new_min(f.dim);
  std::vector<double> new_max(f.dim);
  for (int i = 0; i < f.dim; i++) {
    double opt = f.max_loc[i];
    double max_edge = f.max_edge[i];
    double min_edge = f.min_edge[i];
    //Calculate how much can be removed from max  
    double max_diff = (max_edge - opt)*buffer;
    //Calculate how much can be added to min
    double min_diff = (opt - min_edge)*buffer;

    RandomReal max_rand(rng, UniformRealDist(0,max_diff));
    new_max[i] = max_edge - max_rand();
    RandomReal min_rand(rng, UniformRealDist(0,min_diff));
    new_min[i] = min_edge + min_rand();

    assert(new_min[i] < opt && opt < new_max[i]);
  } 
  Function new_fn = f;
  new_fn.min_edge = new_min;
  new_fn.max_edge = new_max;
  return new_fn;
} /* tweak_edges() */
