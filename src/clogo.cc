#include <cassert>
#include <cmath>

extern "C" {
#include "clogo/clogo.h"
}

#define CAT(a, ...) a ## __VA_ARGS__
#define _FN_MAX(x) CAT(MAX__, x)
#define FN_MAX _FN_MAX(FN)

#define MAX__rosenbrock_2 (0.0)
#define MAX__sin_2 (0.9517936893872353)

#ifndef FN
#define FN rosenbrock_2
#endif

int logo_schedule(
  const struct clogo_state *state
)
{
  static const int w[] = {3, 4, 5, 6, 8, 30};
  int w_cnt = sizeof(w)/sizeof(w[0]);
  int j = -1;
  for (int i = 0; i < w_cnt; i++) {
    if (state->w == w[i]) {
      j = i;
      break;
    }
  }
  assert(j != -1);

  int k = j;
  double new_best = state_best_value(state); 
  if (new_best > state->last_best_value) {
    k++;
  } else {
    k--;
  }

  if (k < 0) k = 0;
  else if (k >= w_cnt) k = w_cnt-1;

  return w[k];
}

int soo_schedule(
  const struct clogo_state *state
)
{
  (void)state; //Don't need to use the parameter if we 
               //always return the same value.
  return 1;
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

double hmax(
  int n                    //current number of function eval
)
{
  return sqrt((double)n);
}

struct clogo_options test_soo()
{
  struct clogo_options opt = { 
    .max = 4000,
    .k = 3,
    .fn = &FN,
    .hmax = &hmax,
    .w_schedule = soo_schedule,
    .init_w = 1,
    .epsilon = 1e-4,
    .fn_optimum = FN_MAX,
  };
  return opt;
}

struct clogo_options test_logo()
{
  struct clogo_options opt = { 
    .max = 4000,
    .k = 3,
    .fn = &FN,
    .hmax = &hmax,
    .w_schedule = logo_schedule,
    .init_w = 3,
    .epsilon = 1e-4,
    .fn_optimum = FN_MAX,
  };
  return opt;
}

