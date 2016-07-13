#include <cassert>
#include <cmath>
#include <iostream>

#include "common.h"
#include "clogo.h"

using std::cout;
using std::endl;

void display_result(const struct clogo_result& result)
{
  cout << result.samples << "\t";
  cout << result.point[0] << "," << result.point[1] << "\t";
  double regret = FN_MAX - result.value;
  cout << regret << endl;
}

void eval(struct clogo_options opt)
{
  struct clogo_state state = clogo_init(&opt);
  while (!clogo_done(&state)) {
    clogo_step(&state);
    struct clogo_result result = clogo_finish(&state);
    display_result(result);
  }
  clogo_delete(&state);
}

void eval_soo()
{
  eval(test_soo());
}

void eval_logo()
{
  eval(test_logo());
}

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

double hmax(
  int n                    //current number of function eval
)
{
  return sqrt((double)n);
}

struct clogo_options test_soo()
{
  struct clogo_options opt = { 
    .max = TOTAL_SAMPLES,
    .k = 3,
    .fn = &FN,
    .hmax = &hmax,
    .w_schedule = soo_schedule,
    .init_w = 1,
    .epsilon = INFINITY,
    .fn_optimum = INFINITY,
  };
  return opt;
}

struct clogo_options test_logo()
{
  struct clogo_options opt = { 
    .max = TOTAL_SAMPLES,
    .k = 3,
    .fn = &FN,
    .hmax = &hmax,
    .w_schedule = logo_schedule,
    .init_w = 3,
    .epsilon = INFINITY,
    .fn_optimum = INFINITY,
  };
  return opt;
}

